//---------------------------------------------------------------------------
#pragma hdrstop
#include <DateUtils.hpp>
#include <stddef.h>
#include <vcl.h>
#include <algorithm>
#include <math.h>
#include "TCalcIndCont.h"
#include "VTCalcIndContSE.h"
#include "VTLigaCrit.h"
#include "TVisRedeSoc.h"
#include "VTAvaliaVis.h"
#include "TLisDefeitoRede.h"
#include "VTVisRede.h"
#include "VTBlocoRad.h"
#include "VTBarraRad.h"
#include "VTEstratSoc.h"
#include "VTResCont.h"
#include "VTBuscaProt.h"
#include "TBuscaProt.h" //hk201808: reformula: temporário
#include "VTDefeito.h"
#include "VTLisBlocoChave.h"
#include "VTLeArqParamTxt.h"
#include "..\BDadoParam\Dados\VTParamConf.h"
#include "..\BDadoParam\Dados\VTVerifCjto.h"
#include "..\BDadoParam\Dados\VTVerifSe.h"
#include "..\BDadoParam\BDado\VTBDadoParam.h"
#include "..\BdadoParam\Modela\VTGeraLisParamEqpto.h"
#include "..\CalcIndCont\VTCalcEstratSoc.h"

#include <PlataformaSinap\Fontes\Compensacoes\VTCompensacoes.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Bloco\VTElo.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>

#include "..\..\DLL_Inc\BDadoParam.h"

#include <PlataformaSinap\DLL_Inc\Compensacoes.h>
#include <PlataformaSinap\DLL_Inc\Bloco.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Log.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//COMENTÁRIOS
   //PENDENCIAS 30/06/15
   //1-Transferir para a aplicação alocação de chaves as funções abaixo:
   //Funcoes acessorias para alocacao de chaves NA
   //         bool   DefChavesNACand(TList *lisExt, TList *lisFora, VTRede *rede1, VTRede *rede2, int nchvCand);
   //         void   LisBlocosDiv(VTRede *rede, TList *lisExt, double eLim);
   //   VTBarraRad*  BarraDivBloco(VTBloco *bloco);

   //ok: 2- Transferir para as aplicações as leituras de arquivos ou de banco de dados
   //   entrada somente por função de lista de parametros
   //   ReiniciaLisParamEqptoExterno ReiniciaLisParamRedeExterno
   //         bool   LeArqCircVerif(AnsiString arqParam);
   //         bool   LeArqOcorrVerif(AnsiString arqParam);

   //Alterações 201607
   //->radial:
   //radial->Inicia(redes); hk201607 transferido para IniciaProblema(NULL)
   //->Inicialização de paramConf->LisParamRede:
   //hk201607--> verifica se já existe paramRede
   //IniciaLisParamRede(); //hk201607: nova inicialização que verifica se já existe paramRede
                         //          e novas redes terão parametros iguais a paramGerais
   //hk201607: novas redes inicializadas com paramGerais
   //   paramRede->lambda  = paramConf->ParamGerais->lambda;//0.1;//0.0;
   //   paramRede->trep    = paramConf->ParamGerais->trep;//50.0;//0.0;
   //->Em IniciaProblema
   //hk201607: caso haja modificacoes de topologia e os blocos são reiniciados
   //entao é feito novo mapeamento de blocos e de carga/redeMT
   //Atribui ponteiro paramBloco->eqpto e paramBloco->bloco e recalcula o comprimento do bloco
   //geraLisParamEqpto->PreparaLisParamBlocoRede(paramConf->LisParamBloco);
   //Atribui ponteiro paramCarga->rede, paramCarga->carga  = carga e paramCarga->nconsJus = NCons(carga);
   //geraLisParamEqpto->PreparaLisParamCargaRede(paramConf->LisParamCarga);
   //->Reinicialização de chaves automáticas
   //Compatibilidadade buscaProt->LisChavesAuto/ paramConf->LisChavesAuto
   //Inicialização de chaves automáticas baseado nas informações de telecomando/ tecnologia da chave no modelo SINAP
   //->   //hk201607 Inicia listas de resultados após inicialização do problema
   //e reinicia resCont
   //para todas as redes carregadas
   //IniciaListasResultados(NULL);
   //-> Proteções na execucao do cálculo
   //Alteracoes hk201608
   //->Em IniciaLisParamRede, Verifica se a rede corresponde a circuito primário, MT //hk201608
   //->Em LeArqMdb,    //hk201608 Reinicia parâmetros default e limpa listas
   //lisParamBloco;lisParamCarga;lisParamRede;lisParamEqpto;lisVerifCjto;lisChavesTelecom;lisTipoEqProt;
   //paramConf->IniciaParamDefault();
   //e reinicia parametros associados a redes não incluídas na base paramConf com valor de taxas e tempos de paramGeral
   //IniciaLisParamRede();
   //->Em IniciaProblema, reinicia chaves telecomandadas religadoras com informações da rede de chaves telecomandadas
   //IniciaChavesTelecomandadasRede();
   //hk201608: retirada a opção de fusão de blocos

   //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede: bool flagEliminaReg = true;

   //201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
   //para  : geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto,paramConf->LisParamRede, paramConf->ParamGerais);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1

   //hk201701: em TCalcIndCont::LeArqMdb(AnsiString arqParam)
   //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
   //if(!radial->ExistePrimario(rede)) continue;

   //hk201701: em TCalcIndCont::IniciaLisDefeitos(VTRede *rede)
   //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
   //if(!radial->ExistePrimario(redeDef)) continue; //hkv201608 verifica se rede é circuito primário

   //hk201701: Pendencia
   //Considerar distância de bloco sem trechos (com uma barra ou transformadores)
   //igual a 1.00

//hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
//IniciaLisParamRede(void)
//LeArqMdb(AnsiString arqParam)
//IniciaLisDefeitos(VTRede *rede)
//hk201701: Caso de um registro com parametros DEFAULT para ajuste de parâmetros cria verificado com todas as redes
//IniciaVerifDefault()
//hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento : Acréscimo de variável resBloco->rede
//IniciaListasResultados(VTRede *rede)
//IniciaLisDefeitos(VTRede *rede)
//ExecutaAtualizaResBloco(TLisDefeitoRede *pDefeitoRede)
//hk201701: problemas em redes AT : Acréscimo de variável resBloco->rede: if(resBloco->bloco->Rede != rede) continue;
//ExecutaCalculoCompensacoes(VTRede *rede)
//CalculaIndicadoresRedeMT(TLisDefeitoRede *pDefeitoRede)
//CalculaIndicadoresRedeBT1(VTRede *rede, int iMes)
//hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
//ExecutaRede(VTRede *rede, bool flagReiniciaRes)
//ExecutaAjusteParametrosLisRede(TList *lisRede, double decMeta, double fecMeta, double &decAjusta, double &fecAjusta, double erroMax, int iteraMax)
//ExecutaAjusteParametrosLisRedeBlocoCarga(TList *lisRede, double decMeta, double fecMeta, double &decAjusta, double &fecAjusta, double erroMax, int iteraMax)
//hk201701: correcao de atualização de erro
//ExecutaAjusteParametrosLisRede(TList *lisRede, double decMeta, double fecMeta, double &decAjusta, double &fecAjusta, double erroMax, int iteraMax)

//hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
//hk201702: teste para execução de isolamentos no cálculo de indicadores: implementação de função que retorna a chave de suprimento do bloco

//hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por bloco
//hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por carga

//DVK 2017.04.11: IniciaParamDefeito - verifica se está zerado
//---------------------------------------------------------------------------
VTCalcIndCont* __fastcall NewObjCalcIndCont(VTApl* apl_owner, TList *lisChavesFunde, bool flagVis)
   {
   return(new TCalcIndCont(apl_owner, lisChavesFunde, flagVis));
   }

//---------------------------------------------------------------------------
__fastcall TCalcIndCont::TCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
   {
   //Cria objetos Iniciais:
   //apl, blocos, radial, buscaProt, avaliaVis, paramConf, resCont, geraLisParamEqpto, log
   IniciaObjetos(apl_owner);

   //Inicia parametros default, paramConf->ParamGerais
   //e  Inicia paramConf->listaParamRede com redes carregadas
   //lambda= trep = 0 e nfalhasEquip= durTotal= ndiasObs = 0
   //ParamDefault();

   //Lista de objetos da class VTDefeito
   lisDefeitos    = new TList();
   //lista de objeto que guarda lista de defeitos por rede
   lisDefeitoRede = new TList();
   //Lista parametros de rede e por bloco
   //lisParamRede   = new TList();
   //lisParamBloco  = new TList();
   //lisParamCarga  = new TList();
   //Lista de parâmetros por equipamento que atuou
   //lisParamEqpto  = new TList();
   //Gera lisCargaBlocos->cBloco->P_mw[iPat = 18]
   lisCargaBlocos = new TList();
   //Lista de chaves automatizadas
   //lisChavesAuto= new TList();
   //Lista de chaves clone com informações de status(p.ex tipo da chave) originais
   lisChavesClone = new TList();
   //Lista de chaves novas - a serem implementadas - Se não pertencem a lisChavesAuto - não considerada
   lisChavesNovas = new TList();
   //Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto)
   //hkedp1 lisChavesAtivas= new TList();
   //hk201808: reformula: lisChavesAtivas = NULL;

   //lista de chaves para fusao de blocos
   this->lisChavesFunde = lisChavesFunde;

   //lisVerifCjto = new TList();
   //Inicia o problema com os dados da rede, Limpa lista de defeitos,
   //Reinicia lista de defeitos por bloco dos alimentadores
   //e reinicia parametros (taxas e tempos de reparo por defeito)
   //para todas as redes carregadas
   IniciaProblema(NULL);
   //hkv201608 Inicia parametros default, paramConf->ParamGerais
   //e  Inicia paramConf->listaParamRede com redes carregadas
   //lambda= trep = 0 e nfalhasEquip= durTotal= ndiasObs = 0
   ParamDefault();
   //hkv201607 Inicia listas de resultados
   //e reinicia resCont
   //para todas as redes carregadas
   IniciaListasResultados(NULL);
   //Calcula folgas das chaves de VIS
   flagVisAvalia = false;
   //Pendencia: ?Rodar o fluxo de potencia somente quando executa o cálculo?
	if (flagVis)
	{
		AvaliaFolgaVis();
	}

   //flag se é realizado cálculo mensal de indicadores
   flagMensal = false;
   //forca calculo anual de compensacoes
   flagForcaAnual = false;

   //Atualiza dados gerais
   //Inicia lista com chaves automáticas e telecomandadas
   //hkv201608: sai reinicialização de chaves auto, que já se encontra em IniciaProblema--> IniciaChavesTelecomandadasRede
   //ReiniciaChavesAuto();//IniciaLisChavesAuto();

   //pathParam = ""; //hk201808: reformula: vai para paramConf
   //IniciaListaParamBT();
   //parConfRec = NewObjParConfRec(apl);

   //IniciaCalcIndContSE();

   }

//---------------------------------------------------------------------------
__fastcall TCalcIndCont::~TCalcIndCont(void)
   {
   LimpaTList(lisChavesClone);
   //fecha log
   log->Close();
	if (apl) {delete apl; apl = NULL; }
	if (lisCargaBlocos) {LimpaTList(lisCargaBlocos); delete lisCargaBlocos; lisCargaBlocos = NULL;}
	if (lisDefeitos) {LimpaTList(lisDefeitos);delete lisDefeitos;lisDefeitos = NULL;}
	if (lisDefeitoRede) {LimpaTList(lisDefeitoRede);delete lisDefeitoRede;lisDefeitoRede = NULL;}
	if (lisChavesClone) {delete lisChavesClone;lisChavesClone = NULL;}
	if (lisChavesNovas) {delete lisChavesNovas;lisChavesNovas = NULL;}
}

//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::IniciaCalcIndContSE(void)  //hk201808: reformula: SAI
   {
   VTRede *redeSE;
   VTCalcIndContSE *calcIndContSe;
   TList *lisSE;
   VTSubestacao *se;

   lisSE = radial->LisSubestacao();

   for(int n = 0; n < lisSE->Count; n++)
      {
      se = (VTSubestacao*)lisSE->Items[n];
		if (se->Rede->Codigo.AnsiCompare("BON") != 0)
			continue;
      calcIndContSe = NewObjCalcIndContSE(apl, se->Rede);
      calcIndContSe->ExecutaIndicadoresSE();
      delete calcIndContSe;
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaObjetos(VTApl *aplOwner)
   {
	VTRedes      *redes   = (VTRedes*)aplOwner->GetObject(__classid(VTRedes));

   //Cria novo apl para a aplicacao
   //this->apl = stk->NewObjApl(NULL, aplOwner);
   this->apl = NewObjApl(NULL, aplOwner);

   //cria e inicia objeto Radial
   //apl->Add(radial = (VTRadial*)stk->NewObjRadial(apl));
   //radial->Inicia(redes);
   //cria objeto da classe blocos
   //this->apl->Add(stk->NewObjBlocos(this->apl));
   this->apl->Add(DLL_NewObjBlocos());

   //Adiciona objeto redes radiais
   //this->apl->Add(radial = (VTRadial*)stk->NewObjRadial(this->apl));
   this->apl->Add(radial = DLL_NewObjRadial(this->apl));
   //radial->Inicia(redes); hk201607 transferido para IniciaProblema(NULL)

   //Cria objeto para buscas topologicas na rede
   this->apl->Add(buscaProt = NewObjBuscaProt(this->apl, true));//true: busca ponteiro de blocos
   //Cria objeto para cálculo de estratégias de socorro
   this->apl->Add(NewObjCalcEstratSoc());//new TBuscaProt(apl));
   //Cria objeto de leitura de chaves do scada
   this->apl->Add(NewObjAvaliaVis(this->apl));

   //Inicia objeto com parametros
   this->apl->Add(paramConf = DLL_NewObjParamConf());

   //objeto com parametros padrões
   //paramDefault = new strParamRede; //ssai
   //Cria objeto para guardar resultados
   this->apl->Add(resCont = NewObjResCont(apl));//hk18

   //Inicia ponteiro para patamares
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   numPat   = patamares->NumPatamar();

   //Cria objeto para preparacao de parametros
   //Trata o conjunto lisParamEqpto
   this->apl->Add(geraLisParamEqpto = DLL_NewObjGeraLisParamEqpto(apl));
   //geraLisParamEqpto->PreparaLisParamEqptoRede(paramConf->LisParamEqpto);
   //delete geraLisParamEqpto;

   //Cria objeto log
   this->apl->Add(log = DLL_NewObjLog(this->apl));
   //Inicia log
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arqLog = path->DirTmp() + "\\LogCalcIndCont.txt";
   log->Open(arqLog, false);//append = false

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaTipoProt(void) //hk18
   {//inicia buscaProt->LisTipoEqProt: default
    //atualiza paramConf->LisTipoEqProt: paramConf->InsereLisTipoChaveProtecaoParamConf(lisTipoEqProt);
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoChave *tipoChave;
   VTChave     *chave;
   TList       *lisChave;
   TList       *lisTipoEqProt = buscaProt->LisTipoEqProt;

   lisTipoEqProt->Clear();
   if(tipos == NULL)
     {
     //Popula a partir da lista de chaves

     lisChave = new TList();
     redes->LisEqpto(lisChave, eqptoCHAVE);
     for(int n = 0; n < lisChave->Count; n++)
        {
        chave = (VTChave*)lisChave->Items[n];
        tipoChave = chave->TipoChave;
			if (lisTipoEqProt->IndexOf(tipoChave) == -1)
				lisTipoEqProt->Add(tipoChave);
        }
     delete lisChave;
     }
   else
     {
     //Popula a partir de tipos

     lisTipoEqProt->Assign(tipos->LisTipoChave(), laOr);
     }

   for(int n = lisTipoEqProt->Count - 1; n >= 0; n--)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];
		if (tipoChave->Tipo == chaveDJ)
			continue;
		if (tipoChave->Tipo == chaveRA)
			continue;
		if (tipoChave->Tipo == chaveBF)
			continue;
      lisTipoEqProt->Remove(tipoChave);
      }
   /*
   chaveDJ=1, chaveDISJUNTOR   =chaveDJ,
   chaveRA,   chaveRELIGADORA  =chaveRA,
   chaveBF,   chaveBASE_FUSIVEL=chaveBF,
   chaveSEC,  chaveSECCIONADORA=chaveSEC,
   chaveINDEFINIDA,
   chaveDEFINIDA_USUARIO,
   //chaves Eletropaulo/Subterrâneo
   chaveDESCONECTAVEL=1000, chaveNETWORK_PROTECTOR
   */

   for(int n = lisTipoEqProt->Count - 1; n >= 0; n--)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];

		if (tipoChave->Codigo.AnsiCompare("Disjuntor") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Religador") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Base Fusível") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Chave Fusivel Repetidora MT") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Seccionalizador") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Chave Fusivel") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Chave Interruptora Tripolar Seca") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Chave Fusivel Especial 34,5kV") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare("Chave de Protecao Subterranea") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare
			("Posto Transformador 34,5kV com eqpto de proteçao/manobra") == 0)
			continue;
		if (tipoChave->Codigo.AnsiCompare
			("Posto Transformador 34,5kV sem eqpto de proteçao/manobra") == 0)
			continue;
      lisTipoEqProt->Remove(tipoChave);
      }
//      tipoChave->Codigo.AnsiCompare("Seccionadora") == 0) continue;
//      if(tipoChave->Codigo.AnsiCompare("Regulador de Tensao") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Faca") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Indefinida") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Autotransformador") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Basculante") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Banco de Capacitores") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Seccionadora MT SF6") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Unipolar") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Adaptada") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Interligaçao 13,8kV/34,5kV") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Seccionamento Operativo") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave de Manobra Subterranea") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Seccionamento de Primario") == 0) continue;

   //Popula lista paramConf->LisTipoEqProt
   paramConf->InsereLisTipoChaveProtecaoParamConf(lisTipoEqProt);//hkreformula: vai para paramConf
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ParamDefault(void) //hk201808: reformula
   {
   //Inicia paramConf e
   //Inicia buscaProt: buscaProt->IProfReliga, buscaProt->LisTipoEqProt
   //PENDENCIA - PASSAR PARA TPARAMCONF
   paramConf->IniciaParamDefault();
   buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;//hkedp
   //Inicia paramConf->LisTipoEqProt e buscaProt->LisTipoEqProt
   IniciaTipoProt();
   //Preenche lisParamRede
   //Limpa a lista
   LimpaTList(paramConf->LisParamRede);
   //Gera paramRede para todas as redes carregadas
   //hkreformula
   VTRedes *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   paramConf->IniciaLisParamRede(redes->LisRede());
   //Inicia Parametros
   //Inicia parametros por rede
   //Inicia vetor de distribuição de falhas por patamar constante: distFalhasPat[npat] = 1/24
   //hkreformula
   paramConf->IniciaDistFalhas(numPat);

   //hk201808: reformula
   /*paramConf->ParamGerais->flagRecurso = true;  //sem uso de recursos a jusante
   paramConf->ParamGerais->flagRetorno = false;
   // nao considera manobras de retorno no cálculo dos indicadores
   paramConf->ParamGerais->tlocaliza   = 25;
   paramConf->ParamGerais->tman_manual = 10;
   paramConf->ParamGerais->tman_auto = 1;
   paramConf->ParamGerais->pu_lim  = 1.1;
   paramConf->ParamGerais->percDefTrans  = 0.8;
   paramConf->ParamGerais->tlimIC = 3;
	// duração máxima (min) do desligamento para não acrescentar os indicadores de contin.
   paramConf->ParamGerais->lambda = 0.1;    //hkdef
   paramConf->ParamGerais->trep   = 50.0;   //hkdef trrepDefault   = 50.0;
   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   paramConf->ParamGerais->ptpre       = 0.0;
   paramConf->ParamGerais->ptdlo       = 0.0;
   paramConf->ParamGerais->ptexe       = 0.0;

	paramConf->ParamGerais->iProfReliga   = 0;
	//gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
	paramConf->ParamGerais->red_tprep_IF = 0.7;
   buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;//hkedp

   //Inicia paramConf->LisTipoEqProt e buscaProt->LisTipoEqProt
   IniciaTipoProt();

   //Preenche lisParamRede
   //Limpa a lista
   LimpaTList(paramConf->LisParamRede);
   //Inicia listaParamRede com redes carregadas //hkv201607--> verifica se já existe paramRede

   //lambda= trep = 0 e nfalhasEquip= durTotal= ndiasObs = 0
   //VTRedes *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   //VTRede  *rede;
   //TList   *lisRede = redes->LisRede();
   //strDadoParamRede *paramRede;
   //Preenche lisParamRede
   //Limpa a lista
   //LimpaTList(paramConf->LisParamRede);
   //for(int n = 0; n < lisRede->Count; n++)
   //   {
   //   rede = (VTRede*)lisRede->Items[n];
   //   if(!rede->Carregada) continue;
   //   paramConf->LisParamRede->Add(paramRede = new strDadoParamRede);
   //   paramRede->rede = rede;
   //   paramRede->compRede = ComprimentoRede(paramRede->rede);
   //   paramRede->codRede = rede->Codigo;
   //   paramRede->lambda  = 0.1;//0.0;
   //   paramRede->trep    = 50.0;//0.0;
   //   }

   //Gera paramRede para todas as redes carregadas
   //hkreformula
   VTRedes *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   paramConf->IniciaLisParamRede(redes->LisRede());
   //IniciaLisParamRede(); //hkv201607: nova inicialização que verifica se já existe paramRede
                         //          e novas redes terão parametros iguais a paramGerais

   //Inicia Parametros
   //Inicia parametros por rede
   //Inicia vetor de distribuição de falhas por patamar constante: distFalhasPat[npat] = 1/24
   //hkreformula
   paramConf->IniciaDistFalhas(numPat);

   //Inicia paramConf->LisVerifCjto
   //hk201802a: IniciaLisVerifCjto();
   */
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::PreparaLisChavesAtivas(void)
   {
   //Executada em:
   //IniciaProblema(VTRede *rede)
   //Executa(void)
   //ExecutaRede(VTRede *rede)
   //ExecutaDicFicPuMensal(void)
   //PM_GetLisChavesAtivas(void)
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTChave     *chave;

   //lisChavesAtivas: Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
   //hk201808: reformula: lisChavesAtivas->Clear();
   buscaProt->LisChavesAtivas->Clear();
   redes->LisEqpto(buscaProt->LisChavesAtivas, eqptoCHAVE);//hk201808: reformula: redes->LisEqpto(lisChavesAtivas, eqptoCHAVE);

   lisChavesNovas->Clear();
   //Acrescenta a lisChavesNovas chaves bloqueadas - IniNova
   for(int nc = 0; nc < buscaProt->LisChavesAtivas->Count; nc++)//hk201808: reformula: for(int nc = 0; nc < lisChavesAtivas->Count; nc++)
      {
      chave = (VTChave*)buscaProt->LisChavesAtivas->Items[nc];//hk201808: reformula: chave = (VTChave*)lisChavesAtivas->Items[nc];
      if(chave->Bloqueada)
        {
        if(lisChavesNovas->IndexOf(chave) == -1)
            lisChavesNovas->Add(chave);
        }
      }

   for(int nc = 0; nc < lisChavesNovas->Count; nc++)
      {
      chave = (VTChave*)lisChavesNovas->Items[nc];
      //Se a chave esta prevista a instalação
		if (buscaProt->LisChvAuto->IndexOf(chave) != -1)
			continue;
      buscaProt->LisChavesAtivas->Remove(chave);//hk201808: reformula: lisChavesAtivas->Remove(chave);
      }

   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCalcIndCont::LeDado(FILE* fp, bool &flagEOF)
   {
   AnsiString x;
   char  aux;

   flagEOF = false;

   x = "";
   aux = fgetc(fp);
   while ((aux  != ';') && (aux  != '\n') && (aux  != EOF))
	{
		x += AnsiString(aux);
                  aux = fgetc(fp);
                 }
	if (aux == EOF)
		flagEOF = true;

   return(x);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RetornaParametrosHistorico(bool flagGeral, bool flagCircuito,
	bool flagEqpto, bool flagDistHora)
   {
//   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
//   AnsiString arq;
//   arq = path->DirTmp() + "\\parentrada.txt";
//   paramConf->ImprimeParametrosEntrada(arq);

   //Verifica conjunto de dados historicos diferente de vazio
	if (paramConf->LisParamEqpto->Count == 0)
		return;

   if(flagGeral)
     {
     //Calcula paramGeral->lambda e paramGeral->trep
     //
     //em funcao de:
     //paramGeral->durTotal
     //paramGeral->nFalhasEquip
     //paramGeral->compRede
     //paramGeral->ndiasObs
     geraLisParamEqpto->ReiniciaDadosParamGeral(paramConf->ParamGerais);
     }
   if(flagCircuito)
     {
     //Calcula paramRede->lambda e paramRede->trep
     //
     //em funcao de:
     //paramRede->durTotal
     //paramRede->nFalhasEquip
     //paramRede->compRede
     //paramRede->ndiasObs
     geraLisParamEqpto->ReiniciaDadosParamRede(paramConf->LisParamRede);
     }

   if(flagEqpto)
     {
     //Reinicia taxas e tempos de reparo
     //lisParamEqpto->paramEqpto->lambda  , trep

     //Funcao de:
     //paramEqpto->durTotal
     //paramEqpto->nFalhasEquip
     //paramEqpto->ndiasObs
     geraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);
     //Reinicia lisParamBloco->paramBloco->(lambda, trep)
     //       e lisParamCarga->paramCarga->(lambda, trep)
     //Funcao de:
     //paramEqpto->lambda
     //paramEqpto->trep
     //Zera a lista de parametros por bloco
     //hkmarlimpa LimpaTList(paramConf->LisParamBloco);
		geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
		// hkmar (paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);

     }
   if(flagDistHora)
     {
     //Reinicia paramConf->DistFalhasPat
     //em funcao de paramGerais->nfalhasHora[0..23]
     geraLisParamEqpto->ReiniciaDistribHora(paramConf->ParamGerais, paramConf->DistFalhasPat);
     }

   //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
   IniciaParamDefeito();
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::PreparaParamConfRede(void)//hkreformula: funcao complementar preparaParamConfRede
   {
   VTRedes     *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
   //1.PARAMCIRC atribui ponteiro de rede
   //paramRede->rede, paramRede->compRede
   //2.atribui ponteiros a lisParamEqpto, eqpto/ carga e gera distribuição de falhas
   //e gera paramEqpto->nfalhasEquip, durTotal
   //                 ->nfEquip[nm], durTot[nm]
   //3.Redistribui falhas para somar 1.0, soma distFalhasPat[0->23] = 1.0
   //4.Atribui ponteiro paramBloco->eqpto e paramBloco->bloco, paramBloco->compBloco
   //5.Atribui ponteiro paramCarga->rede (MT), paramCarga->carga  = carga e paramCarga->nconsJus = NCons(carga);
   //6.PARAMCHAVESTELECOM atribui ponteiro chvTelecom->chave de chave existente na rede
   //7.TIPOEQPROT atribui ponteiros dadoTipoEqProt->tipoChave de tipos de chave
   geraLisParamEqpto->PreparaParamConfRede(paramConf, NULL);
   //Seta em buscaProt variável de profundidade de religamento
   buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;
   //Insere em buscaProt->LisTipoEqProt equipamentos de protecao de paramConf->LisTipoEqProt
   InsereLisTipoChaveProtecaoBuscaProt(paramConf->LisTipoEqProt);
   //Insere chaves telecomandadas -->reinicia buscaProt->LisChvAuto
   //e reinicia blocos agregados por chaves novas, pdef->LisBlocosAgrega
	InsereLisChavesTelecom(paramConf->LisChavesTelecom);
   //Reinicia e Atualiza buscaProt->LisChavesIF com paramConf->LisChavesIF
	InsereLisChavesIF(paramConf->LisChavesIF);
	// gh201809: inicia as chaves com flag de telecomando na rede
	IniciaChavesTelecomandadasRede();

	//double* distFalhas = paramConf->DistFalhasPat;
   //for(int n = 0; n < 24; n++) distFalhasPat[n] = distFalhas[n];

   //hkv201608a Reinicia parametros associados a redes não incluídas na base
   //paramConf, mas carregadas na plataforma SINAP,
   //com valor de taxas e tempos de paramGeral
   paramConf->IniciaLisParamRede(redes->LisRede());//hkreformula: IniciaLisParamRede();

   //hkv201608a Caso não haja observações no banco de parametros inicia lisVerifCjto
   //com parametros default
   //hk201701: Caso de um registro com parametros DEFAULT para ajuste de parâmetros cria verificado com todas as redes
	//if (paramConf->LisVerifCjto->Count < 2)
	//hk201802a: IniciaVerifDefault();
   //if(paramConf->LisVerifCjto->Count == 0) IniciaVerifDefault();

   //Calculo de paramRede
      //paramGeral\paramRede->nFalhasEquip
      //paramGeral\paramRede->durTotal
      //paramGeral\paramRede->ndiasObs
      //paramGeral\paramRede->nFEquip[nm]
      //paramGeral\paramRede->durTot[nm]
      //paramGeral\paramRede->cli, clih
      //paramGeral\paramRede->climes[nm], clihmes[nm]
   //Funcao de
      //paramEqpto->tipoRede
      //paramEqpto->rede
      //paramEqpto->nFalhasEquip;
      //paramEqpto->durTotal;
      //paramEqpto->ndiasObs;
      //paramEqpto->nFEquip[nm]
      //paramEqpto->durTot[nm]
      //paramEqpto->cli, clih
      //paramEqpto->climes[nm], clihmes[nm]
   //201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
	geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto, paramConf->LisParamRede,
		paramConf->ParamGerais);
	// ->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1

   //Reinicia taxas e tempos de reparo lisParamEqpto->paramEqpto(lambda, trep)
   //Funcao de:
   //paramEqpto->clih, paramEqpto->cli - versão anterior: paramEqpto->durTotal
   //paramEqpto->nFalhasEquip
   //paramEqpto->ndiasObs
   geraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);

   //Reinicia parametros de defeito com taxas lidas da base de parametros, paramConf.mdb
   //Atualiza baseado em lisParamBloco, lisParamRede, paramGeral:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramGeral->lambda;
   //--->--->pdef->TReparo   = paramGeral->trep;
   IniciaParamDefeito();

   //Gera dic e fic pu_mensal com dados observados, flaMensal: existem 12 meses com observação
   //flagForcaAnual = true; //hktemp
   if(!flagForcaAnual) //hk201703: forca o calculo simplificado de compensacao
      flagMensal = ExecutaDicFicPuMensal();


   //VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov
   //FILE      *fout;
   //AnsiString arq;
   //arq = path->DirTmp() + "\\ParamSimula_LeArqMdb.txt";
   //paramConf->ImprimeParametrosSimula(arq);
   //Reinicia DICpu FICpu

   return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereLisChavesTelecom(TList *lisChavesTelecomExterno)
   {
   VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
   strDadoChaveTelecom *chvTelecom;
   //Inicia lista com chaves automáticas e telecomandadas buscaProt->LisChvAuto
   //a partir de lista de chaves de paramConf->LisChavesTelecom
   //ReiniciaChavesAuto();//hkv201608 sai ReiniciaChavesAuto e reinicia apenas
   //buscaProt->LisChvAuto
   VTChave *pchave;
   while(buscaProt->LisChvAuto->Count > 0)//modBuscaProt
        {
        pchave = (VTChave*)buscaProt->LisChvAuto->First();
        //Remove chave de buscaProt->LisChvAuto copia atributos de clone e elimina chaveClone
        RemoveChaveAutoBuscaProt(pchave);       //hkv201608
        }
   //gh201809
   //IniciaChavesTelecomandadasRede();//hkiniciaTelecom
   for(int n = 0; n < lisChavesTelecomExterno->Count; n++)
      {
      chvTelecom = (strDadoChaveTelecom*)lisChavesTelecomExterno->Items[n];
      //Considera apenas chaves identificadas nas redes carregadas
		if (chvTelecom->chave == NULL)
			continue;
      //----------Insere chave em lisChavesAuto--------------
      //-e altera o tipo da chave para chaveRA, religadora---
      //e cria chave clone para retorno a configuração original
		//gh201809
		InsereChaveAuto(chvTelecom->chave, false);
      }
   //Reinicia blocos agregados por chaves novas, pdef->LisBlocosAgrega
   GeraLisBlocosDefeito();
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ReiniciaChavesAuto(void)
   {
   //Funcao que zera lista de chaves automáticas

   //hkv201608: já reinicia chave com clone e deleta em lisChavesClones em RemoveChaveAuto
   /*VTChave *pchave, *pchaveClone;
   //Reinicia chaves previamente clonadas em lisChavesClone
   //busca pchave em lisChavesClone
   for(int n = lisChavesClone->Count - 1; n >= 0 ; n--)
      {
      pchaveClone = (VTChave*)lisChavesClone->Items[n];
      pchave = (VTChave*)pchaveClone->Obj;
      pchave->CopiaAtributosDe(*pchaveClone);
      }
   */
   VTChave *pchave;
   while(buscaProt->LisChvAuto->Count > 0)
        {
        pchave = (VTChave*)buscaProt->LisChvAuto->First();
        //Remove de chave de buscaProt->LisChvAuto
        //Remove de lisChavesAtivas, caso esteja em lisChavesNovas (chave nova)
        //Retorna os atributos da chave original guardada em chaveClone
        //altera  lisChavesClone e deleta chaveClone
		  // gh201809
		  RemoveChaveAuto(pchave, true);
        }

   //hkedp buscaProt->LisChvAuto->Clear();

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RemoveChaveAuto(VTChave *pchave, bool flagParamConf)
   {
	//gh201809
	//Remove chave de buscaProt->LisChvAuto copia atributos de clone e elimina chaveClone
	RemoveChaveAutoBuscaProt(pchave);       //hkv201608

	//Remove chave auto de paramConf: paramConf->LisChavesTelecom
	if (flagParamConf)
		{
		strDadoChaveTelecom *chvTelecom;
		for(int n = paramConf->LisChavesTelecom->Count - 1; n >= 0; n--)
			{
			chvTelecom = (strDadoChaveTelecom*)paramConf->LisChavesTelecom->Items[n];
			if (chvTelecom->codChave.AnsiCompare(pchave->Codigo) != 0)
				continue;
		  paramConf->LisChavesTelecom->Delete(n);
		  delete chvTelecom;
		  }
		}
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RemoveChaveAutoBuscaProt(VTChave *pchave)  //hkv201608
   {
   //Funcao para:
   //remover chave de buscaProt->LisChvAuto
   //copiar atributos de clone e
   //eliminar chaveClone

   VTChave *pchaveClone;

	if (buscaProt->LisChvAuto->IndexOf(pchave) == -1)
		return;
	while (buscaProt->LisChvAuto->IndexOf(pchave) != -1)
		buscaProt->LisChvAuto->Remove(pchave);

   //Caso seja chave nova remove de lista de chaves ativas hkedp
	if (lisChavesNovas->IndexOf(pchave) != -1)
		buscaProt->LisChavesAtivas->Remove(pchave);//hk201808: reformula: lisChavesAtivas->Remove(pchave);

   //busca pchave em lisChavesClone
   for(int n = lisChavesClone->Count - 1; n >= 0 ; n--)
      {
      pchaveClone = (VTChave*)lisChavesClone->Items[n];
		if (((VTChave*)(pchaveClone->Obj)) != pchave)
			continue;
      //*pchave << *pchaveClone;
      pchave->CopiaAtributosDe(*pchaveClone);
      lisChavesClone->Delete(n);
      delete pchaveClone;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereChavesIF(AnsiString arq)
	{
	VTBDadoParam* bdadoParam;
	TStringList *lisGrupoSensores;
	AnsiString codSensor;
   strDadoChaveIF *chaveIF;

	lisGrupoSensores = new TStringList();
	bdadoParam = DLL_NewObjBDadoParam(apl);
	//gh201806: preenche lista de sensores existentes na rede elétrica, a partir do arquivo selecionado
	bdadoParam->ImportaDadoChaveIF(lisGrupoSensores, arq);
   delete bdadoParam; //hk201808: reformulação

   //Inicia lista de parametros sensores, paramConf->LisChavesIF
   LimpaTList(paramConf->LisChavesIF);

	for (int i = 0; i < lisGrupoSensores->Count; i++)
		{
		codSensor = (lisGrupoSensores->Strings[i]).Trim();
      chaveIF = NULL;
      for(int ns = 0; ns < paramConf->LisChavesIF->Count; ns++)
         {
         chaveIF = (strDadoChaveIF*)paramConf->LisChavesIF->Items[ns];
         if(chaveIF->codChave.AnsiCompare(codSensor) == 0) break;
         chaveIF = NULL;
         }
      if(chaveIF == NULL)
        {
        paramConf->LisChavesIF->Add(chaveIF = new strDadoChaveIF);
        chaveIF->codChave = codSensor;
        }
      }
   DestroiTObject(lisGrupoSensores);
   //Atribui ponteiro de chave
   geraLisParamEqpto->PreparaLisChavesIF(paramConf->LisChavesIF);
   //Atualiza buscaProt->LisChavesIF->(chave)
   InsereLisChavesIF(paramConf->LisChavesIF);



   //hk201808: reformulação
	/*
	TList *lisChaves;
   lisChaves = new TList();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTChave *chaveIF;
	redes->LisEqpto(lisChaves, eqptoCHAVE);
	//gh201806: varre a lista de chaves com IFs cadastrados e de chaves da rede para adicionar IF nas chaves
	for (int i = 0; i < lisGrupoSensores->Count; i++)
		{
		codSensor = lisGrupoSensores->Strings[i];
		for (int j = 0; j < lisChaves->Count; j++)
			{
			chaveIF = (VTChave*)lisChaves->Items[j];
			if (codSensor != NULL)
				{
				if (codSensor.AnsiCompare(chaveIF->Codigo) == 0)
					{//adiciona chave e chvIF = new strDadoChaveIF em buscaProt->LisChavesIF e paramConf->LisChavesIF
					InsereChaveIF(chaveIF);
					}
				}
			}
		}
	lisGrupoSensores->Clear();
	DestroiTObject(lisGrupoSensores);
	DestroiTObject(lisChaves);
   */
	}
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereLisChavesIF(TList *lisChavesIF)
	{//ModBuscaProt
	TList *lisChaves;
	strDadoChaveIF *chaveIF;
	VTChave *chave;

	//lisChaves = new TList();
	//VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	//redes->LisEqpto(lisChaves, eqptoCHAVE);

   //Reinicia lista de chaves com IF
   buscaProt->LisChavesIF->Clear();

	//gh201806: varre a lista de chaves com IFs cadastrados e de chaves da rede para adicionar IF nas chaves
	for (int i = 0; i < lisChavesIF->Count; i++)
		{
		chaveIF = (strDadoChaveIF*)lisChavesIF->Items[i];
      if(chaveIF->chave == NULL) continue;        //hk201808: reformulação
      if(buscaProt->LisChavesIF->IndexOf(chaveIF->chave) == -1)
         buscaProt->LisChavesIF->Add(chaveIF->chave);//hk201808: reformulação
		////hk201808: reformulação
      //if (chaveIF != NULL)
		//	{
		//	for (int j = 0; j < lisChaves->Count; j++)
		//		{
		//		chave = (VTChave*)lisChaves->Items[j];
		//		if (chaveIF->chave == chave)
		//			{
		//			InsereChaveIF(chave);
		//			}
		//		}
		//	}
		}
	//DestroiTObject(lisChaves);
	}
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RemoveChaveIF(VTChave *pchave)
	{
	VTChave *chave;
	strDadoChaveIF *chvIF;
	buscaProt->LisChavesIF->Remove(pchave);
	for (int i = paramConf->LisChavesIF->Count - 1; i >= 0 ; i--)
		{
		chvIF = (strDadoChaveIF*)paramConf->LisChavesIF->Items[i];
		if (chvIF->codChave.AnsiCompare(pchave->Codigo) != 0)
			continue;
		paramConf->LisChavesIF->Delete(i);
		delete chvIF;
		}
   }
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereChaveAuto(VTChave *pchave, bool flagParamConf, bool flagRA) //hk201710: flagRA= true, mudança de tipo de chave para religadora
	{
	//gh201809
	VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoChave  *tipoReliga = NULL;
   VTTipoChave  *tipoDisjuntor = NULL;
   VTTipoChave  *tipoChave; //hk201612
   VTChave *pchaveClone;

	if (buscaProt->LisChvAuto->IndexOf(pchave) != -1)
		return;

   //busca pchave em lisChavesClone
   //busca pchave em lisChavesClone
   for(int n = lisChavesClone->Count - 1; n >= 0 ; n--)
      {
      pchaveClone = (VTChave*)lisChavesClone->Items[n];
		if (((VTChave*)pchaveClone->Obj) != pchave)
			continue;
      lisChavesClone->Delete(n);
      delete pchaveClone;
      }

   //InsereChave em lisChavesClone
   lisChavesClone->Add(pchaveClone = pchave->Clone());
   pchaveClone->Obj = (TObject*)pchave;

   buscaProt->LisChvAuto->Add(pchave);
   //Grava a informação de telecomando em chave
   if(pchave->Telecomandada == false) //hk201808: reformula
     {
     pchave->TecnoComunicacao = comRADIO;
     pchave->Telecomandada = true;
     }
   tipoChave = pchave->TipoChave;
   //hk201612: flagRA: se muda o tipo para religadora
   //if((pchave->TipoChave->Tipo != chaveRA)&&(pchave->TipoChave->Tipo != chaveDJ))
   if(flagRA)
     {
   if((tipoChave->Tipo != chaveRA)&&(tipoChave->Tipo != chaveDJ))
     {
     tipoReliga = tipos->ExisteTipoChave(chaveRA);
		if (tipoReliga)
			pchave->TipoChave = tipoReliga;
     }
     }

   //Insere chave na lista de chaves ativas hkedp
   if(buscaProt->LisChavesAtivas->IndexOf(pchave) == -1)//hk201808: reformula
                         buscaProt->LisChavesAtivas->Add(pchave);

	//Insere chave auto de paramConf: paramConf->LisChavesTelecom, apenas se não for chave telecomandada da rede
	if (flagParamConf)
		{
		strDadoChaveTelecom *chvTelecom = NULL;
		for(int n = paramConf->LisChavesTelecom->Count - 1; n >= 0; n--)
			{
			chvTelecom = (strDadoChaveTelecom*)paramConf->LisChavesTelecom->Items[n];
			if (chvTelecom->codChave.AnsiCompare(pchave->Codigo) == 0)
				break;
			chvTelecom = NULL;
			}
		if(chvTelecom == NULL)
			{
			paramConf->LisChavesTelecom->Add(chvTelecom = new strDadoChaveTelecom);
			chvTelecom->codChave = pchave->Codigo;
			chvTelecom->chave = pchave;
			}
		}

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaProblema(VTRede *rede)
   {
   //Funcao que reinicia problema em função de mudanças na rede
   //sem reiniciar parâmetros iniciais

   //Executada em:
   //TSistemaAL::ReiniciarCalcIndCont(void)
   //TCalcIndCont::TCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
   //Executa(TList *lisInsercao, TList *lisSubstituicao)
   //TFormCalcIndCont::ActionReiniciarExecute(TObject *Sender)
   VTRedes      *redes;
   TList *lisDef;

   //hkv201607 radial->Inicia(redes) transferido de IniciaObjetos para IniciaProblema(NULL)
   if(rede == NULL)
      {
      redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
      radial->Inicia(redes);
      }

   //hkedp1 buscaProt->LisChavesAtivas = lisChavesAtivas;
   //hk201808: reformula: lisChavesAtivas = buscaProt->LisChavesAtivas;
   //hkv201608 reinicia chaves telecomandadas religadoras com informações da rede de chaves telecomandadas
   //Insere novas chaves telecomandadas e atualiza buscaProt->LisChvAuto e lisChavesClone com chaves com informação de telecomando
   IniciaChavesTelecomandadasRede();
   //Prepara novamente lista de chaves ativas hkedp
   //Lista de chaves consideradas : lisChavesAtivas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
   PreparaLisChavesAtivas();
   /*  //hkv201608: retirada a opção de fusão de blocos
   bool flagFunde = false;
   //lisChavesFunde: chaves na rede final com eliminacao de chaves e fusao de blocos
   //se há fusao lisChavesDaRede->Count > lisChavesFunde->Count
   if(lisChavesFunde != NULL)
     {
     redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
     TList *lisChaves;
     lisChaves = new TList();
     redes->LisEqpto(lisChaves, eqptoCHAVE);
     if(lisChaves->Count > lisChavesFunde->Count) flagFunde = true;
     delete lisChaves;
     //hkv201608 if(lisChavesFunde->Count > 0) flagFunde = true;   //errado
     }
   if(flagFunde)
     {
     //Funde blocos limitados por lisChavesFunde
     //blocos->Executa
     //Funde blocos de chaves não existentes em lisChavesFunde
     FundeBlocos(lisChavesFunde);
     if(!buscaProt->Inicia(false)) ////(bool flagExeBlocos = false): não executa blocos->Executa
      {
       //AnsiString aux = IntToStr(lisChavesFunde->Count);
       //Aviso(aux);
       Aviso("Problemas na inicialização: verificar dados de entrada");//OrdenaChaves();
       return;
      }
     }
   else
     {//Inicia buscaProt Ordenacao de chaves e identifica chaves VIS
     if(!buscaProt->Inicia()) ////(bool flagExeBlocos = true)
      {Aviso("Problemas na inicialização: verificar dados de entrada");//OrdenaChaves();
       return;
      }
     }
   */

	if (!buscaProt->Inicia())
		////(bool flagExeBlocos = true)
	{
		Aviso("Problemas na inicialização: verificar dados de entrada"); // OrdenaChaves();
      return;
     }

   //hk201710IF //hk201808: reformula
   buscaProt->PreparaIndicacaoFalta();

   //hkv201608: preparação de buscaProt
   //------------------------------------------------------------
   //Calcula carga dos blocos e número de consumidores por bloco
   //blocoRad->CargaBloco[np] - carga de blocoRad->bloco
   //blocoRad->CargaBlRad[np] - carga do de blocoRad->bloco e blocos a jusante
   //------------------------------------------------------------
   buscaProt->CalculaCargasBlocoRad();

   //Em caso de novos blocos ou mudancas de tipo das chaves e
   //consequente mudanca nas zonas de religamento hkedp

   //Sem ReGeração de  lisParamEqpto->paramEqpto->(lambda, trep) = f(paramEqpto->nFalhasEquip, durTotal, ndiasObs)
   //para não perder o ajuste, através de ReiniciaDadosParamEqp

   //hkv201607: Para novas redes inseridas cria paramConf->LisParamRede com parametro geral
   //hkreformula
   redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   paramConf->IniciaLisParamRede(redes->LisRede());
   //IniciaLisParamRede();

   //hkv201607: Em IniciaProblema
   //hkv201607: caso haja modificacoes de topologia e os blocos são reiniciados
   //entao é feito novo mapeamento de blocos e de carga/redeMT
	bool flagEliminaReg = true;
	// hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede: bool flagEliminaReg = true;
   //Em funcao de paramBloco->codChave, Atribui ponteiro paramBloco->eqpto e paramBloco->bloco e recalcula o comprimento do bloco
	geraLisParamEqpto->PreparaLisParamBlocoRede(paramConf->LisParamBloco, flagEliminaReg);
	// flagEliminaReg: se elimina registros nao encontrados na rede de paramConf->LisParamBloco
   //Em funcao de paramCarga->codCarga, Atribui ponteiro paramCarga->rede, paramCarga->carga  = carga e paramCarga->nconsJus = NCons(carga);
	geraLisParamEqpto->PreparaLisParamCargaRede(paramConf->LisParamCarga, flagEliminaReg);
	// flagEliminaReg: se elimina registros nao encontrados na rede de paramConf->LisParamCarga
   //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede

   //e IniciaParamDefeito()    limpa lista de defeitos de rede e gera pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   IniciaLisDefeitos(rede);

   //Inicia] resCont
   //Deixa calculada a demanda por bloco: Gera lisCargaBlocos->cBloco->P_mw[iPat = 18]
   //e total de carga radial: blocoRad->CargaBlRad[np] e blocoRad->NCons
   //blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   //CalculaCargaBlocos(rede); //saihk201607

   //------------------------------------------------------------
   //Calcula carga dos blocos e número de consumidores por bloco
   //blocoRad->CargaBloco[np] - carga de blocoRad->bloco
   //blocoRad->CargaBlRad[np] - carga do de blocoRad->bloco e blocos a jusante
   //------------------------------------------------------------
   //hkv201608: preparação de buscaProt buscaProt->CalculaCargasBlocoRad();

   //Inicia listas de resultados
   //IniciaListasResultados(rede); hk201607 sai
   //Inicia treeview
   //TVInicia();

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaChavesTelecomandadasRede(void)  //hkv201608
   {
   //gokh201809
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTChave *chave;
   TList   *lisChaves;

   //Funcao que Insere novas chaves telecomandadas religadoras
   //com informações da rede de chaves telecomandadas
   lisChaves = new TList();
   redes->LisEqpto(lisChaves, eqptoCHAVE);
   for(int nc = 0; nc < lisChaves->Count; nc++)
      {
      chave = (VTChave*)lisChaves->Items[nc];
		if (chave->Telecomandada == true)
         {
			InsereChaveAuto(chave, false);
         }
      //else
      //   RemoveChaveAuto(chave);//hkiniciaTelecom
      }

   delete lisChaves;
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaListasResultados(VTRede *rede)
   {
   //se flagMensal = true é necessário guardar
   //strResCarga->dic_pu[nm], fic_pu[nm] já calculados
   if(flagMensal == true)
     {
     TList *lisResCargaCopia;
     strResCarga *resCarga, *resCargaCopia;
     lisResCargaCopia = new TList();
     for(int n = 0; n < resCont->LisResCarga->Count; n++)
        {
        resCarga = (strResCarga*)resCont->LisResCarga->Items[n];
        lisResCargaCopia->Add(resCargaCopia = new strResCarga);
        resCargaCopia->carga = resCarga->carga;
        resCarga->carga->Obj = resCargaCopia;
        for(int nm = 0; nm < 12; nm++)
           {
           resCargaCopia->dic_pu[nm] = resCarga->dic_pu[nm];
           resCargaCopia->fic_pu[nm] = resCarga->fic_pu[nm];
           }
        }
     //------------------------------------------------------------
     //Inicia objeto que armazena resultados
     //lisResRede->resRede
     //------------------------------------------------------------
     strResBloco *resBloco;
     resCont->IniciaListas(rede);   //problema: destroi poderadores de dic e fic mensal por carga!
     //Preenche ponteiro resBloco->blocoRad
     for(int n = 0; n < resCont->LisResBloco->Count; n++)
        {
		resBloco = (strResBloco*)resCont->LisResBloco->Items[n];
        resBloco->blocoRad = buscaProt->IdentificaBlocoRad(resBloco->bloco);
		//hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
        if((resBloco->rede == NULL)&&(resBloco->blocoRad != NULL))
           {
           if(resBloco->blocoRad->Chave != NULL)
                        resBloco->rede = resBloco->blocoRad->Chave->rede;
           }
        //barra = (VTBarra*)(resBloco->bloco->LisBarra()->First());
        //barra->Obj = resBloco->blocoRad; //temporariamente
        }
     //Reinicia dic_pu, fic_pu
     for(int n = 0; n < resCont->LisResCarga->Count; n++)
        {
        resCarga = (strResCarga*)resCont->LisResCarga->Items[n];
        resCargaCopia = (strResCarga*)(resCarga->carga->Obj);
        for(int nm = 0; nm < 12; nm++)
           {
           resCarga->dic_pu[nm] = resCargaCopia->dic_pu[nm];
           resCarga->fic_pu[nm] = resCargaCopia->fic_pu[nm];
           }
        }
     LimpaTList(lisResCargaCopia);
     delete lisResCargaCopia;
     }
   else
     {
     //------------------------------------------------------------
     //Inicia objeto que armazena resultados
     //lisResRede->resRede
     //------------------------------------------------------------
     strResBloco *resBloco;
     resCont->IniciaListas(rede);   //problema: destroi poderadores de dic e fic mensal por carga!
     //Preenche ponteiro resBloco->blocoRad
     for(int n = 0; n < resCont->LisResBloco->Count; n++)
        {
		resBloco = (strResBloco*)resCont->LisResBloco->Items[n];
        resBloco->blocoRad = buscaProt->IdentificaBlocoRad(resBloco->bloco);
        //barra = (VTBarra*)(resBloco->bloco->LisBarra()->First());
        //barra->Obj = resBloco->blocoRad; //temporariamente
        }
     }
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaLisDefeitos(VTRede *rede)
   {
   //Executada em
   //IniciaProblema(VTRede *rede)
   VTBlocos    *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede  *redeDef;
   VTDefeito *pdef;
   TList     *lisBloco;
   VTBloco   *blocoDef;
   TList     *lisRedes;
   TLisDefeitoRede *lDefRede;

   if(rede == NULL)
     {
     //Reinicia lista de defeitos
     LimpaTList(lisDefeitos);
     //Reinicia lista de objetos com listas de defeitos de uma rede
     LimpaTList(lisDefeitoRede);
     }
   else
     {//objetos VTDefeito e TLisDefeitoRede referentes a rede != NULL
     for(int ndr = lisDefeitoRede->Count - 1; ndr >= 0; ndr--)
        {
			if ((lDefRede = (TLisDefeitoRede*)lisDefeitoRede->Items[ndr])->rede == rede)
				continue;
        for(int ndef = 0; ndef < lDefRede->lisDefeitos->Count; ndef++)
           {
           pdef = (VTDefeito*)lDefRede->lisDefeitos->Items[ndef];
           lisDefeitos->Remove(pdef);
           delete pdef;
           }
        lisDefeitoRede->Delete(ndr);
        delete lDefRede;
        }
     }

   lisRedes = new TList();
   if(rede == NULL)
     lisRedes->Assign(redes->LisRede(), laOr);
   else
     lisRedes->Add(rede);

   lisBloco = new TList();//blocos->LisBloco();
   for(int nr = 0; nr < lisRedes->Count; nr++)
      {
      redeDef = (VTRede*)lisRedes->Items[nr];
		if (!redeDef->Carregada)
			continue;
      //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
      //if(!radial->ExistePrimario(redeDef)) continue; //hkv201608 verifica se rede é circuito primário

      lisBloco->Clear();
      //Preenche lista com blocos ordenados da montante a jusante //Descarta blocos com rede = NULL
      buscaProt->ListaOrdenadaBlocos(lisBloco, redeDef);
		if (lisBloco->Count == 0)
			continue;
      lisDefeitoRede->Add(lDefRede = new TLisDefeitoRede(redeDef));

      VTBlocoRad *blocoRad;
      for(int nb = 0; nb < lisBloco->Count; nb++)
         {
         blocoDef = (VTBloco*)lisBloco->Items[nb];
         lisDefeitos->Add(pdef = NewObjDefeito(blocoDef, numPat));
         lDefRede->lisDefeitos->Add(pdef);
			pdef->ComprimentoBloco = ComprimentoBloco(pdef->Bloco);
			// Comprimento: blocos compostos por 1 equipamento?
         //hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento
         if(pdef->Rede == NULL)
           {
           if((blocoRad = buscaProt->IdentificaBlocoRad(blocoDef)) != NULL)
              {
					if (blocoRad->Chave != NULL)
						pdef->Rede = blocoRad->Chave->rede;
              }

           }
         }
      }

   //Reinicia blocos agregados por defeito
   GeraLisBlocosDefeito(rede);

   //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramDefault->lambda;
   //--->--->pdef->TReparo   = paramDefault->trep;
   IniciaParamDefeito();

   delete lisRedes;
   delete lisBloco;
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaParamDefeito(void)
   {
   //Funcao(paramRede, paramBloco, paramDefault)
   //que recalcula paramaetros por defeito em bloco
   //pdef->TReparo, pdef->TaxaFalha
   TList                *lisParamRede = paramConf->LisParamRede;
   strDadoParamRede     *paramRede;
   strDadoParamBloco    *paramBloco, *paramBlocoAgrega;
   TLisDefeitoRede      *lisDefRede;
   VTDefeito            *pdef, *pdefAgrega;
   double                trepCirc, lambdaCirc;
   double                ptpreCirc, ptdloCirc, ptexeCirc;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   VTBloco              *blocoAgrega;
   TList                *lisDefeitoAux;

   //Atualiza baseado em lisParamBloco, lisParamRede, paramGeral:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramGeral->lambda;
   //--->--->pdef->TReparo   = paramGeral->trep;
   flagParBloco     = true;   //hkverif
   flagParCircBloco = true;

   //Cria lista de defeitos auxiliar
   lisDefeitoAux = new TList();

   //mudarr parou aqui parametro mes em iparamdefeito
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      lisDefRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      //Busca parametros por circuito em this->lisParamRede
		paramRede = NULL;
		for(int np = 0; np < lisParamRede->Count; np++)
			{
			paramRede = (strDadoParamRede*)lisParamRede->Items[np];
			if (paramRede->rede == lisDefRede->rede)
				break;
				paramRede = NULL;
			}
      //hk201707: Sugestão DVK
      /*paramRede = NULL;
      for(int np = 0; np < lisParamRede->Count; np++)
         {
         paramRede = (strDadoParamRede*)lisParamRede->Items[np];
         if(paramRede->rede == lisDefRede->rede)  //break;
            {//(DVK 2017.04.11 verifica se está zerado)
            if((! IsDoubleZero(paramRede->lambda)) && (! IsDoubleZero(paramRede->trep)))
               break;
            else paramRede = NULL;
            }
         else paramRede = NULL;
         }
      */
      if(paramRede == NULL) //mudarr lambdadefault
        {
        lambdaCirc = paramConf->ParamGerais->lambda;
        trepCirc   = paramConf->ParamGerais->trep;
        //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
        ptpreCirc  = paramConf->ParamGerais->ptpre;
        ptdloCirc  = paramConf->ParamGerais->ptdlo;
        ptexeCirc  = paramConf->ParamGerais->ptexe;
        }
      else
        {
        //hk201707: Sugestão DVK se os lambda = trep = 0 entao atribui valores de paramGeral
        if((IsDoubleZero(paramRede->lambda)) && (IsDoubleZero(paramRede->trep)))
			{
				lambdaCirc = paramConf->ParamGerais->lambda;
				trepCirc = paramConf->ParamGerais->trep;
			}
        else //parametros específicos
			{
				lambdaCirc = paramRede->lambda;
				trepCirc = paramRede->trep;
			}
        //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
        ptpreCirc  = paramRede->ptpre;
        ptdloCirc  = paramRede->ptdlo;
        ptexeCirc  = paramRede->ptexe;
        }

      for(int ndef = 0; ndef < lisDefRede->lisDefeitos->Count; ndef++)
         {
         pdef = (VTDefeito*)lisDefRede->lisDefeitos->Items[ndef];
         //Verifica se já percorreu defeito em blocos agregados
			if (lisDefeitoAux->IndexOf(pdef) != -1)
				continue;

         if(flagParBloco || flagParCircBloco)
           {
           pdef->TaxaFalha = lambdaCirc;//0.0;          //hk18.1
           pdef->TReparo   = trepCirc;
           pdef->Ptpre     = 0.0;//pdef->Ptpre     = ptpreCirc;    admite o valor geral
           pdef->Ptdlo     = 0.0;//pdef->Ptdlo     = ptdloCirc;
           pdef->Ptexe     = 0.0;//pdef->Ptexe     = ptexeCirc;
           //busca strParamBloco em this->lisParamBloco
           //paramBloco = ParametroConfBloco(pdef->Bloco); //passar blocos agregados

           paramBloco = paramConf->ParamBloco[pdef->Bloco];
           if(paramBloco != NULL)
				{
					pdef->TaxaFalha = paramBloco->lambda;
               pdef->TReparo   = paramBloco->trep;
               //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
               pdef->Ptpre     = paramBloco->ptpre;
               pdef->Ptdlo     = paramBloco->ptdlo;
               pdef->Ptexe     = paramBloco->ptexe;
               if(!pdef->FlagTPre)
                 {
                 pdef->Ptpre = ptpreCirc;
                 pdef->Ptdlo = ptdloCirc;
                 pdef->Ptexe = ptexeCirc;
                 }

               pdef->FatTMA    = paramBloco->fatTMA;//hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por bloco
               //Passa para blocos vizinhos
               if(pdef->LisBlocosAgrega->Count > 1) //se houver blocos vizinhos
                 {
                 for(int nbl = 0; nbl < pdef->LisBlocosAgrega->Count; nbl++)
                    {
                    blocoAgrega = (VTBloco*)pdef->LisBlocosAgrega->Items[nbl];
							if (blocoAgrega == pdef->Bloco)
								continue;
                    //Busca defeito relativo a bloco
                    pdefAgrega = BuscaDefeitoRede(blocoAgrega, lisDefRede);
							if (pdefAgrega == NULL)
								continue;
                    pdefAgrega->TaxaFalha = paramBloco->lambda;
                    pdefAgrega->TReparo   = paramBloco->trep;

                    //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
                    pdefAgrega->Ptpre     = paramBloco->ptpre;
                    pdefAgrega->Ptdlo     = paramBloco->ptdlo;
                    pdefAgrega->Ptexe     = paramBloco->ptexe;

                    lisDefeitoAux->Add(pdefAgrega);
                    }
                 }

              }
           else
              {
              if(flagParCircBloco)
                {
                //Calcula número de falhas médio
                pdef->TaxaFalha = lambdaCirc;
                //pdef->NFalhas = lambdaCirc * ComprimentoBloco(pdef->Bloco);
                pdef->TReparo = trepCirc;
                //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
                pdef->Ptpre     = 0.0;//pdef->Ptpre     = ptpreCirc;   admite o valor geral
                pdef->Ptdlo     = 0.0;//pdef->Ptdlo     = ptdloCirc;
                pdef->Ptexe     = 0.0;//pdef->Ptexe     = ptexeCirc;
                }
              }
           }
         else
           {
           //Calcula número de falhas médio
           pdef->TaxaFalha = lambdaCirc;
           //pdef->NFalhas = lambdaCirc * ComprimentoBloco(pdef->Bloco);
           pdef->TReparo = trepCirc;
           //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
           pdef->Ptpre     = 0.0;//pdef->Ptpre     = ptpreCirc;    admite o valor geral
           pdef->Ptdlo     = 0.0;//pdef->Ptdlo     = ptdloCirc;
           pdef->Ptexe     = 0.0;//pdef->Ptexe     = ptexeCirc;
           }
         //   tman = 5;    //min
         //   trep = 50;   //min
         //   lambda = 0.1;//falhas/km/ano
         //hk18.1 pdef->ComprimentoBloco = ComprimentoBloco(pdef->Bloco);
         }
      }
   delete lisDefeitoAux;
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaParamDefeitoMes(int iMes)
   {
   //Funcao(paramRede, paramBloco, paramDefault)
   //que recalcula paramaetros por defeito em bloco
   //pdef->TReparo, pdef->TaxaFalha
   TList *lisParamRede = paramConf->LisParamRede;
   strDadoParamRede *paramRede;
   strDadoParamBloco    *paramBloco;
   TLisDefeitoRede  *lisDefRede;
   VTDefeito        *pdef;
   double           trepCirc, lambdaCirc;

	if (iMes < 0)
		return;

   flagParBloco     = true;   //hkverif
   flagParCircBloco = true;

   //mudarr parou aqui parametro mes em iparamdefeito

   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      lisDefRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      //Busca parametros por circuito em this->lisParamRede
      paramRede = NULL;
      for(int np = 0; np < lisParamRede->Count; np++)
         {
         paramRede = (strDadoParamRede*)lisParamRede->Items[np];
			if (paramRede->rede == lisDefRede->rede)
				break;
            paramRede = NULL;
         }
      if(paramRede == NULL) //mudarr lambdadefault
		{
			lambdaCirc = paramConf->ParamGerais->lambda * paramConf->ParamGerais->lamb_pu[iMes];
         trepCirc   = paramConf->ParamGerais->trep   * paramConf->ParamGerais->tr_pu[iMes];
        }
      else
		{
			lambdaCirc = paramRede->lambda * paramRede->lamb_pu[iMes];
         trepCirc = paramRede->trep     * paramRede->tr_pu[iMes];
        } //parametros específicos

      for(int ndef = 0; ndef < lisDefRede->lisDefeitos->Count; ndef++)
         {
         pdef = (VTDefeito*)lisDefRede->lisDefeitos->Items[ndef];
         if(flagParBloco || flagParCircBloco)
           {
           pdef->TaxaFalha = lambdaCirc;//hkmar 0.0;          //hkverif
           pdef->TReparo   = trepCirc;
           //busca strParamBloco em this->lisParamBloco
           //paramBloco = ParametroConfBloco(pdef->Bloco); //passar blocos agregados
           paramBloco = paramConf->ParamBloco[pdef->Bloco];
           if(paramBloco != NULL)
				{
					pdef->TaxaFalha = paramBloco->lambda * paramBloco->lamb_pu[iMes];
               pdef->TReparo   = paramBloco->trep   * paramBloco->tr_pu[iMes];
              }
           else
              {
              if(flagParCircBloco)
                {
                //Calcula número de falhas médio
                pdef->TaxaFalha = lambdaCirc;
                //pdef->NFalhas = lambdaCirc * ComprimentoBloco(pdef->Bloco);
                pdef->TReparo = trepCirc;
                }
              }
           }
         else
           {
           //Calcula número de falhas médio
           pdef->TaxaFalha = lambdaCirc;
           //pdef->NFalhas = lambdaCirc * ComprimentoBloco(pdef->Bloco);
           pdef->TReparo = trepCirc;
           }
         //   tman = 5;    //min
         //   trep = 50;   //min
         //   lambda = 0.1;//falhas/km/ano
         pdef->ComprimentoBloco = ComprimentoBloco(pdef->Bloco);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereLisTipoChaveProtecaoBuscaProt(TList *lisTipoEqProtExterno) //hk201808: Atualiza buscaProt->LisTipoEqProt
   {
   //Reinicia lista de buscaProt
   strDadoTipoEqProt *dadoTipoEqProt;

   buscaProt->LisTipoEqProt->Clear();
   for(int n = 0; n < lisTipoEqProtExterno->Count; n++)
      {
      dadoTipoEqProt = (strDadoTipoEqProt*)lisTipoEqProtExterno->Items[n];
		if (dadoTipoEqProt->tipoChave == NULL)
			continue;
      if(buscaProt->LisTipoEqProt->IndexOf(dadoTipoEqProt->tipoChave) == -1)
                                   buscaProt->LisTipoEqProt->Add(dadoTipoEqProt->tipoChave);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ReiniciaTipoEqProt(void)//hk201808: reformula: sai: atualiza paramConf->LisTipoEqProt
   {
   //Reinicia tipos de equipamento de protecao em paramConf->LisTipoEqProt
   paramConf->InsereLisTipoChaveProtecaoParamConf(buscaProt->LisTipoEqProt);//hkreformula: vai para paramConf
   return(true);
   }

//---------------------------------------------------------------------------
VTDefeito* __fastcall TCalcIndCont::BuscaDefeitoRede(VTBloco *bloco, TLisDefeitoRede *lisDefRede)
   {
   VTDefeito *pdef = NULL;
   for(int ndef = 0; ndef < lisDefRede->lisDefeitos->Count; ndef++)
      {
      pdef = (VTDefeito*)lisDefRede->lisDefeitos->Items[ndef];
		if (pdef->Bloco == bloco)
			break;
      pdef = NULL;
      }
   return(pdef);
   }


//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ReinciaLisDefeitos(VTRede *rede)
   {
   //Reinicia listas da classe TDefeito
   //Reinicia estratégisas de socorro: LimpaTList(lisEstratSoc);
   //Define os instantes padrôes
   //Inicia chave de isolamento de montante             : chvIsolaMont     = NULL;
   //Inicia chave de isolamento automático a montante : chvIsolaAutoMont = NULL;
   //Inicia chave de protecao que atuou                : chvProt = chvReliga = NULL;
   //Inicia número de falhas : nfalhas  = 0.0;
   //Inicia número de piscadas por religamento média por ano no bloco do defeito : npisca   = 0.0;
   //Inicia resultados: dcons    = tcons    = nmanAuto = tcons    = 0.0
   //Inicia resultados intermediarios:
   //Man*.tempo = cli = clih, nBlocosManobra = 0. chaveAbre = chaveFecha = NULL
   //ManProtAut, ManAutoDef, ManProtDef, ManReparo[np], ManSocRapida[np], ManSocLenta[np]

   //Reinicia todas as variáveis dos defeitos sem destruir listas
   VTDefeito *pdef;
   for(int nd = 0; nd < lisDefeitos->Count; nd++)
      {
      pdef = (VTDefeito*)lisDefeitos->Items[nd];
      if(rede != NULL)
		{
			if (pdef->Rede != rede)
				continue;
		}
      pdef->Inicia();
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::AvaliaFolgaVis(void)
   {
   VTAvaliaVis     *avaliaVis= (VTAvaliaVis*)apl->GetObject(__classid(VTAvaliaVis));
   bool           flagExecFluxo;
   bool           resposta;
   TList          *lisChvVis;
   //Cria para cada visRede
   //->pchave->pbarra1: (circuito de pbarra1 é o socorredor (que socorre)
   //                     ->ligaCrit1->lisDelta[0->npat]-> delta[ipat]-> (pliga, folga = dmax(A), ia_nom(A))
   //->pchave->pbarra2: (circuito de pbarra2 é o socorredor (que socorre)
   //                     ->ligaCrit2->lisDelta[0->npat]-> delta[ipat]-> (pliga, folga = dmax(A), ia_nom(A))

   //Executa o fluxo e define a folga para cada patamar para o circuito socorredor
   avaliaVis->FlagAvaliaChave = false;
   resposta = avaliaVis->ExecutaVisRede(buscaProt->LisVisRede, flagExecFluxo = true);

   flagVisAvalia = resposta;//hkedp

   return(resposta);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaEstrategia(VTDefeito *pdef)
   {
   pdef->Inicia();
   //hk201808: reformula sai
   //ReGera lista de blocos do defeito
   //BuscaBlocosAgreg(pdef->Bloco, pdef->LisBlocosAgrega);

   //Gera para cada defeito os blocos agregados considerando chaves na rede mas nao ativadas
   /////////////////////////////////////////////////////////////////////////////////////////
   //Funcao que preenche lista com conjunto de blocos vizinhos a bloco ligados e com o//////
   //proprio bloco//////////////////////////////////////////////////////////////////////////
   //por chaves nao ativas//////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////
   buscaProt->BuscaBlocoAgreg(pdef->Bloco, pdef->LisBlocosAgrega);

	if ((paramConf->ParamGerais->flagRecurso) && (!flagVisAvalia))
	{
		AvaliaFolgaVis();
	}
   //Calcula indicadores de defeito
   CalculaIndicadoresDefeitoCenario(pdef);//hk201808: indisp: temp: CalculaIndicadoresDefeito(pdef);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaCompensacoes(VTRede *rede)
   {
   strResRede  *resGeral, *resRede;
   TLisDefeitoRede *pDefeitoRede;

   //Busca parametros de rede  hk201703: soma de resGeral->compensacao = soma[resRede=>Comensacao]
   resGeral= resCont->ResGeral;
   resGeral->compensacao = 0.0;
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      if(rede != NULL)
		{
			if (pDefeitoRede->rede != rede)
				continue;
		}
      if(flagMensal)
         ExecutaCalculoCompensacoesMensal(pDefeitoRede->rede);
      else
         ExecutaCalculoCompensacoes(pDefeitoRede->rede);

      //if(rede != NULL)
		{
			resRede = resCont->ResRede[pDefeitoRede->rede];
         resGeral->compensacao += resRede->compensacao;
        }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaCompensacoesMensal(VTRede *rede)
   {
   TLisDefeitoRede *pDefeitoRede;

   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      if(rede != NULL)
		{
			if (pDefeitoRede->rede != rede)
				continue;
		}
      ExecutaCalculoCompensacoesMensal(pDefeitoRede->rede);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaCalculoCompensacoesMensal(VTRede *rede)
   {
   VTRadial     *radial   = (VTRadial*)apl->GetObject(__classid(VTRadial));
   VTCompensacoes *compensacao;
   VTSubestacao *pse = NULL;
   TList *lisResCargaRede, *lisSE;
   double *dicMensal, *ficMensal;
   strResCarga *resCarga;
   strResRede  *resRede;

   //Busca parametros de rede
   resRede = resCont->ResRede[rede];
	if (resRede == NULL)
		return (true);
	if (resRede->ncons == 0)
		return (true);
   //Aviso("lisSE = radial->LisSubestacao();" + rede->Codigo);
   lisSE = radial->LisSubestacao();
   pse = NULL;
   for(int nse = 0; nse < lisSE->Count; nse++)
      {
      pse = (VTSubestacao*)lisSE->Items[nse];
		if (pse->ExistePrimario(rede))
			break;
      pse = NULL;
      }

   //Aviso("AnsiString aux" + rede->Codigo);
   AnsiString aux;
   if(pse == NULL)
     {
		aux.sprintf("Subestação não localizada de Primário: %s", rede->Codigo.c_str());
		// hk201612: atualiza log
     log->Add(aux);
     //hk201612 Aviso("Subestação não localizada");
     return(false);
     }

   lisResCargaRede = resCont->LisResCargaRede[rede];
   //resCont->LisResCargaRede(lisResCargaRede, rede);
	if (lisResCargaRede->Count == 0)
	{
		return (false);
	}

   dicMensal = new double[12];
   ficMensal = new double[12];
   compensacao = DLL_NewObjCompensacoes(NULL, apl, paramConf->PathParam);//Empresa);//hk201808: reformula: vai para paramConf
   for(int n = 0; n < lisResCargaRede->Count; n++)
      {
      resCarga = (strResCarga*)lisResCargaRede->Items[n];
      for(int nm = 0; nm < 12; nm++)
         {
         dicMensal[nm] = resCarga->dic * resCarga->dic_pu[nm];
         ficMensal[nm] = resCarga->fic * resCarga->fic_pu[nm];
         }

      resCarga->compensacao = compensacao->Calcula(pse, resCarga->carga, dicMensal, ficMensal);
      resRede->compensacao += resCarga->compensacao;
      }
   delete compensacao;
   delete[] dicMensal;
   delete[] ficMensal;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaCalculoCompensacoes(VTRede *rede)
	{
	VTRadial     *radial   = (VTRadial*)apl->GetObject(__classid(VTRadial));
	VTCompensacoes *compensacao;
	strParamComp   paramComp;
	VTCarga *carga = NULL;
	VTSubestacao *pse = NULL;
	double dcomp;
	TList     *lisSE, *lisCargas;
	strResRede  *resRede;
	strResBloco *resBloco;
	//strParamRede  *paramRede;
	strDadoParamCarga *paramCarga;
	//strParamEqpto *paramEqpto;
	double      dic, fic, dic_bt, fic_bt;
	//VTExtratorDados_Externo_DLL *extrator;
	int noc;
	double tma;
	int ncargaVIP;
	double compAux, dicBT, dicMT, ficBT, ficMT;

	//Busca parametros de rede
	resRede = resCont->ResRede[rede];
	if (resRede == NULL)
		return (true);

	resRede->compensacao  = 0.0;
	resRede->ncargaVIP    = 0.0;
	resRede->dicVIP       = 0.0;
	resRede->ficVIP       = 0.0;
	if (resRede->ncons == 0)
		return (true);

	//Busca parametros de rede
	//if((paramRede = ParametroConf(rede)) == NULL) paramRede = paramDefault;

	//Limpa lista com resultados por carga
	//Aviso("strResCarga *resCarga" + rede->Codigo + "(" + IntToStr(resCont->LisResCarga->Count)+")");
	strResCarga *resCarga;
	TList       *lisResCargaRede;
	//for(int n = resCont->LisResCarga->Count -1; n >=0; n--)
	//   {
	//   resCarga = (strResCarga*)resCont->LisResCarga->Items[n];
	//   if(resCarga->rede != rede) continue;
	//   resCont->LisResCarga->Delete(n);
	//   delete resCarga;
	//   }
	//Reinicia indicadores de resCont->LisResCarga
	//lisResCargaRede = new TList();
	lisResCargaRede = resCont->LisResCargaRede[rede];
	//resCont->LisResCargaRede(lisResCargaRede, rede);
	if (lisResCargaRede->Count == 0)
	{
		return (false);
	}
	for(int n = 0; n < lisResCargaRede->Count; n++)
		{
		resCarga = (strResCarga*)lisResCargaRede->Items[n];
		resCarga->dic   = 0.0;
		resCarga->dicBT = 0.0;
		}

//   FILE  *fout;
//   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
//   AnsiString arq;
//   arq = path->DirTmp()+"\\comp_cargas.CSV";
//   if(!FileExists(arq))
//                      {fout = fopen(arq.c_str(), "wt");
//                       fprintf(fout, "Rede; Rede; Carga; nb; numCargas; nc; EnergiaCarga_KwhDia; ");
//                       fprintf(fout, "NCTot; NCRural; NCResid; NCOutros; NCIndust; NCComerc; NCA4; ");
//                       fprintf(fout, "Compens; dic; dicBT; dicMT; fic; ficBT; ficMT; ");
//                       fprintf(fout, "TUSD_B1; TUSD_B2; TUSD_B3; TUSD_BT; TUSD_Final; ");
//                       fprintf(fout, "NaoUrbanoDICAnual; NaoUrbanoFICAnual; UrbanoDICAnual; UrbanoFICAnual; ");
//                       fprintf(fout, "kei; energia\n");
//                      }
//   else
//       fout = fopen(arq.c_str(), "a");

//Rede, Rede, Carga, nb, numCargas, nc, EnergiaCarga,
//NCTot, NCRural, NCResid, NCOutros, NCIndust, NCComerc, NCA4,
//Compens, dic, dicBT, dicMT, fic, ficBT, ficMT,
//TUSD_B1, TUSD_B2, TUSD_B3, TUSD_BT, TUSD_Final,
//NaoUrbanoDICAnual, NaoUrbanoFICAnual, UrbanoDICAnual, UrbanoFICAnual,
//kei, energia

	//extrator    = DLL_CEMIG_NewExtratorDados_Externo(apl);
	compensacao = DLL_NewObjCompensacoes(NULL, apl, paramConf->PathParam);//Empresa);//hk201808: reformula: vai para paramConf

	//Busca pse
	//Aviso("lisSE = radial->LisSubestacao();" + rede->Codigo);
	lisSE = radial->LisSubestacao();
	pse = NULL;
	for(int nse = 0; nse < lisSE->Count; nse++)
		{
		pse = (VTSubestacao*)lisSE->Items[nse];
		if (pse->ExistePrimario(rede))
			break;
		pse = NULL;
		}

	//Aviso("AnsiString aux" + rede->Codigo);
	AnsiString aux;
	lisCargas = new TList();
	if(pse == NULL)
	  {
	  //Aviso("Subestação não localizada");
	  }
	else
	  {
	  for(int nb= 0; nb < resCont->LisResBloco->Count; nb++)
		  {
		  resBloco = (strResBloco*)resCont->LisResBloco->Items[nb];
			if (resBloco->ncons == 0)
				continue; // protecao
		  //hk201701: problemas em redes AT : Acréscimo de variável resBloco->rede: if(resBloco->bloco->Rede != rede) continue;
			if (resBloco->rede != rede)
				continue;
		  lisCargas->Clear();
		  resBloco->bloco->LisEqbar(lisCargas, eqptoCARGA);

		  for(int nc = 0; nc < lisCargas->Count; nc++)
			  {
			  carga = (VTCarga*)lisCargas->Items[nc];

			  //PreencheParam(VTCarga *carga, int ano, int &numOcorr, double &horas)
			  //pendenga extrator->PreencheParam(carga, 2013, noc, tma);  //pendenga !!
			  //Executa compensações regutórias-> loop para todas as carga       arc
			  //dic = dic_bt + resBloco->tdesl / resBloco->ncons/ 60.;
			  //fic = fic_bt + resBloco->ndesl / resBloco->ncons;

			  //dic = tma*noc + resBloco->tdesl /60;/// resBloco->ncons/ 60.;
			  dic = dicMT = resBloco->tdesl /60;
			  fic = ficMT = resBloco->ndesl;
			  //busca paramCarga
			  //if((paramCarga = ParametroConfCarga(carga)) != NULL)
			  if((paramCarga = paramConf->ParamCarga[carga]) != NULL)
				 {
				 //Calculo da contribuição ao dic proveniente de falhas nas redes BT
				 dicBT = paramCarga->trep * paramCarga->lambda/60;
				 ficBT = paramCarga->lambda;
				 dic += dicBT;
				 fic += ficBT;
				 }

			  compAux = compensacao->Calcula(pse, carga, dic, fic);
			  resRede->compensacao += compAux;

			  //fprintf(fout, "%s\t %6.2f\t %6.2f\t  %6.2f\t  %i\n", carga->Codigo, dic, fic, tma, noc);
			  //Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC)

			  //compensacao->Parametros(pse, carga, paramComp);
//           fprintf(fout, "%s; %s; %s; ", pse->Rede->Codigo, rede->Codigo, carga->Codigo);
//           fprintf(fout, "%i; ", nb);
//           fprintf(fout, "%i; ", lisCargas->Count);
//           fprintf(fout, "%i; ", nc);
//           fprintf(fout, "%6.2f; ", CalculaEnergiaCarga(carga) * 1000);
//           fprintf(fout, "%i; ", carga->NumConsTotal);
//           fprintf(fout, "%i; ", carga->NumConsRural);
//           fprintf(fout, "%i; ", carga->NumConsResidencial);
//           fprintf(fout, "%i; ", carga->NumConsOutros);
//           fprintf(fout, "%i; ", carga->NumConsIndustrial);
//           fprintf(fout, "%i; ", carga->NumConsComercial);
//           fprintf(fout, "%i; ", carga->NumConsA4);
//           fprintf(fout, "%8.4f; ", compAux);
//           fprintf(fout, "%6.2f; ", dic);
//           fprintf(fout, "%6.2f; ", dicBT);
//           fprintf(fout, "%6.2f; ", dicMT);
//           fprintf(fout, "%6.2f; ", fic);
//           fprintf(fout, "%6.2f; ", ficBT);
//           fprintf(fout, "%6.2f; ", ficMT);
//           fprintf(fout, "%6.2f; ", paramComp.TUSD_B1);
//           fprintf(fout, "%6.2f; ", paramComp.TUSD_B2);
//           fprintf(fout, "%6.2f; ", paramComp.TUSD_B3);
//           fprintf(fout, "%6.2f; ", paramComp.TUSD_BT);
//           fprintf(fout, "%6.2f; ", paramComp.TUSD_Final);
//           fprintf(fout, "%6.2f; ", paramComp.NaoUrbanoDICAnual);
//           fprintf(fout, "%6.2f; ", paramComp.NaoUrbanoFICAnual);
//           fprintf(fout, "%6.2f; ", paramComp.UrbanoDICAnual);
//           fprintf(fout, "%6.2f; ", paramComp.UrbanoFICAnual);
//           fprintf(fout, "%6.2f; ", paramComp.kei);
//           fprintf(fout, "%6.2f\n", paramComp.energia);

			  //Conta consumidor VIP
			  if(carga->VeryImportant)
				{
					resRede->ncargaVIP += 1.0;
				  resRede->dicVIP    += dic;  //deslig x hora
				  resRede->ficVIP    += fic;  //deslig
				 }

			  //hk18.1 resCont->LisResCarga->Add(resCarga = new strResCarga);
			  resCarga = resCont->ResCarga[carga][lisResCargaRede];
			  lisResCargaRede->Remove(resCarga);

			  resCarga->carga = carga;
			  resCarga->bloco = resBloco->bloco;
			  resCarga->rede  = rede;
			  resCarga->dic   = dic;
			  resCarga->fic   = fic;
			  resCarga->dicBT = dicBT;
			  resCarga->ficBT = ficBT;
			  resCarga->compensacao = compAux;
			resCarga->dmic = CalculaDMIC(dic, fic);

			  }
		  }
	  }
	//Calcula a média dos indicadores por rede com VIP
//   if(!IsDoubleZero(resRede->ncargaVIP))
//     {
//     resRede->dicVIP /= resRede->ncargaVIP;
//     resRede->ficVIP /= resRede->ncargaVIP;
//     }

	delete compensacao;
	delete lisCargas;
	//delete lisResCargaRede;
	return(true);
	}

//---------------------------------------------------------------------------
double __fastcall TCalcIndCont::CalculaDMIC(double dic, double fic)
{
	if(IsDoubleZero(dic) || IsDoubleZero(fic))
	{
		return 0.0;
	}
	else
	{
		return dic/fic;
	}
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15755250
//https://sinapsisenergia.teamwork.com/#tasks/18215471
bool __fastcall TCalcIndCont::Executa(TList *lisInsercao, TList *lisSubstituicao)
	{
	VTBloco *bloco;
	strDadoParamBloco *paramBloco;
	strDadoParamRede  *paramRede;
	double            compEqpto, lambdaEqpto;
	double            compBlocoObra, lambdaBlocoObra;
	VTRede            *rede;
	strBlocoObra	  *blocoObra;
	TList             *lisParamBloco;
	//Realiza atualização de parametros
	//struct strDadoInsercao :
	//TObject {VTEqpto *eqpto; double  taxa_falha;};
	// struct strDadoSubstituicao :
	// TObject {VTEqpto *eqpto_inserido;    VTEqpto *eqpto_retirado;   double  variacao_taxa_falha_perc;};
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	IniciaProblema(NULL);
	IniciaListasResultados(NULL);
	AvaliaFolgaVis();
	lisParamBloco = new TList();
//	lisParamBloco = paramConf->LisParamBloco;

//	for (int i = 0; i < lisParamBloco->Count; i++)
//	{
//		bloco = (VTBloco*)lisParamBloco->Items[i];
//		paramBloco = paramConf->ParamBloco[bloco];
//		if ((paramBloco = paramConf->ParamBloco[bloco]) == NULL)
//		{
//			paramConf->LisParamBloco->Add(paramBloco = new strDadoParamBloco);
//			paramBloco->fatTMA = 1.0;
//			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma por bloco
//			paramBloco->bloco = bloco;
//			paramBloco->codRede = rede->Codigo; // hkv201608paramBloco-Rede
//			paramBloco->compBloco = ComprimentoBloco(bloco);
//			paramBloco->lisObrasBloco = NULL;
//			paramRede = NULL;
//			if (bloco->Rede != NULL)
//				paramRede = paramConf->ParamRede[bloco->Rede];
//			if (paramRede == NULL)
//			{
//				paramBloco->trep = paramConf->ParamGerais->trep;
//				paramBloco->lambda = paramConf->ParamGerais->lambda;
//			}
//			else
//			{
//				paramBloco->trep = paramRede->trep;
//				paramBloco->lambda = paramRede->lambda;
//			}
//		}
//		paramBloco->lisObrasBloco = new TList();
//		paramBloco->lisObrasBloco = NULL;
//	}

	// Altera taxas de falha para Insercao e Substituicao
	if (lisInsercao != NULL) {
		strDadoInsercao *insercao;
		for (int n = 0; n < lisInsercao->Count; n++) {
			insercao = (strDadoInsercao*)lisInsercao->Items[n];
			// Verifica se equipamento inserido é do tipo trecho
			if (insercao->eqpto->Tipo() != eqptoTRECHO)
				continue;
			compEqpto = ((VTTrecho*)insercao->eqpto)->Comprimento_km;
			rede = ((VTTrecho*)insercao->eqpto)->rede; // hkv201608paramBloco-Rede
			// Verifica se comprimento do equipamento é zero
			if (IsDoubleZero(compEqpto))
				continue; // Verif201701
			// busca o bloco de insercao->eqpto
			if ((bloco = buscaProt->IdentificaBloco(insercao->eqpto)) == NULL)
				continue;

			// Verifica a existencia de paramBloco
			if ((paramBloco = paramConf->ParamBloco[bloco]) == NULL) {
				paramConf->LisParamBloco->Add(paramBloco = new strDadoParamBloco);
				paramBloco->fatTMA = 1.0;
				// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma por bloco
				paramBloco->codRede = rede->Codigo; // hkv201608paramBloco-Rede
				paramBloco->bloco = bloco;
				paramBloco->compBloco = ComprimentoBloco(bloco);
				paramBloco->lisObrasBloco = NULL;
				paramRede = NULL;
				if (bloco->Rede != NULL)
					paramRede = paramConf->ParamRede[bloco->Rede]; // Verif201701
				if (paramRede == NULL)
                {
					paramBloco->trep = paramConf->ParamGerais->trep;
					paramBloco->lambda = paramConf->ParamGerais->lambda;
				}
				else
                {
					paramBloco->trep = paramRede->trep;
					paramBloco->lambda = paramRede->lambda;
				}
			}
			if (paramBloco->lisObrasBloco != NULL)
			{
				paramBloco->lisObrasBloco->Add(blocoObra = new strBlocoObra);
				blocoObra->comprimento_eqpto = compEqpto;
				blocoObra->lambda_eqpto = insercao->taxa_falha;
			}
			else
			{
				lisParamBloco->Add(bloco);
				paramBloco->lisObrasBloco = new TList();
				paramBloco->lisObrasBloco->Add(blocoObra = new strBlocoObra);
				blocoObra->comprimento_eqpto = compEqpto;
				blocoObra->lambda_eqpto = insercao->taxa_falha;
			}
			//paramBloco->lambda = (paramBloco->compBloco - compEqpto) * paramBloco->lambda + compEqpto * insercao->taxa_falha;
			//paramBloco->lambda /= paramBloco->compBloco;
		}
	}

	// Altera taxas de falha para Insercao e Substituicao
	if (lisSubstituicao != NULL) {
		strDadoSubstituicao *substituicao;
		for (int n = 0; n < lisSubstituicao->Count; n++) {
			substituicao = (strDadoSubstituicao*)lisSubstituicao->Items[n];
			// Verifica se equipamento inserido é do tipo trecho

			if (substituicao->eqpto_inserido->Tipo() != eqptoTRECHO)
				continue;
			compEqpto = ((VTTrecho*)substituicao->eqpto_inserido)->Comprimento_km;
			rede = ((VTTrecho*)substituicao->eqpto_inserido)->rede; // hkv201608paramBloco-Rede
			// busca o bloco de insercao->eqpto
			if ((bloco = buscaProt->IdentificaBloco(substituicao->eqpto_inserido)) == NULL)
				continue;
			// Verifica a existencia de paramBloco
			if ((paramBloco = paramConf->ParamBloco[bloco]) == NULL) {
				paramConf->LisParamBloco->Add(paramBloco = new strDadoParamBloco);
				paramBloco->fatTMA = 1.0;
				// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma por bloco
				paramBloco->bloco = bloco;
				paramBloco->codRede = rede->Codigo; // hkv201608paramBloco-Rede
				paramBloco->compBloco = ComprimentoBloco(bloco);
				paramBloco->lisObrasBloco = NULL;
				paramRede = NULL;
				if (bloco->Rede != NULL)
					paramRede = paramConf->ParamRede[bloco->Rede];
				if (paramRede == NULL) {
					paramBloco->trep = paramConf->ParamGerais->trep;
					paramBloco->lambda = paramConf->ParamGerais->lambda;
				}
				else {
					paramBloco->trep = paramRede->trep;
					paramBloco->lambda = paramRede->lambda;
				}
			}
			if (paramBloco->lisObrasBloco != NULL)
			{
				paramBloco->lisObrasBloco->Add(blocoObra = new strBlocoObra);
				blocoObra->comprimento_eqpto = compEqpto;
				if (substituicao->variacao_taxa_falha_perc > 100)
				{
					substituicao->variacao_taxa_falha_perc = 100;
				}
				blocoObra->lambda_eqpto = paramBloco->lambda * (1 - substituicao->variacao_taxa_falha_perc * 0.01);
			}
			else
			{
				lisParamBloco->Add(bloco);
				paramBloco->lisObrasBloco = new TList();
				paramBloco->lisObrasBloco->Add(blocoObra = new strBlocoObra);
				blocoObra->comprimento_eqpto = compEqpto;
                if (substituicao->variacao_taxa_falha_perc > 100)
				{
					substituicao->variacao_taxa_falha_perc = 100;
				}
				blocoObra->lambda_eqpto = paramBloco->lambda * (1 - substituicao->variacao_taxa_falha_perc * 0.01);
			}
			// Calcula a proporcao do equipamento no comprimento total do bloco
			// 16.02.2018 - correção: variacao_taxa_falha_perc não vem em porcentagem
			//lambdaEqpto = (1 - substituicao->variacao_taxa_falha_perc / 100) * paramBloco->lambda;
			//paramBloco->lambda = (paramBloco->compBloco - compEqpto) * paramBloco->lambda + compEqpto * lambdaEqpto;
			//paramBloco->lambda /= paramBloco->compBloco;
		}
	}

	for (int i = 0; i < lisParamBloco->Count; i++)
	{
		lambdaBlocoObra = 0;
		compBlocoObra = 0;
		bloco = (VTBloco*)lisParamBloco->Items[i];
		paramBloco = paramConf->ParamBloco[bloco];
		if (paramBloco->lisObrasBloco != NULL)
		{
			for (int j = 0; j < paramBloco->lisObrasBloco->Count; j++)
			{
				blocoObra = (strBlocoObra*)paramBloco->lisObrasBloco->Items[j];
				//calculo da parcela de lambda associada aos equipamentos que foram alterados pela obra, por bloco
				lambdaBlocoObra = lambdaBlocoObra + blocoObra->comprimento_eqpto * blocoObra->lambda_eqpto;
				//calculo do comprimento dos equipamentos que foram alterados pela obra, por bloco
				compBlocoObra = compBlocoObra + blocoObra->comprimento_eqpto;
			}
		//acrescenta a parcela de lambda associada aos blocos que não foram alterados pela obra
		paramBloco->lambda = paramBloco->lambda*(paramBloco->compBloco - compBlocoObra) + lambdaBlocoObra;
		//calcula o lambda medio ponderado pelos comprimentos de cada trecho
		if (paramBloco->compBloco != NULL)
			{
			paramBloco->lambda = paramBloco->lambda/paramBloco->compBloco;
			}
		LimpaTList(paramBloco->lisObrasBloco);
		DestroiTObject(paramBloco->lisObrasBloco);
        paramBloco->lisObrasBloco = NULL;
		}
	}
	// atualiza as taxas por defeito
	IniciaParamDefeito();
	// Executa cálculo de indicadores de continuidade e compensações
	// para todas as redes
	Executa();
	// Executa cálculo de compensações para todas as redes
	ExecutaCompensacoes(NULL);
	DestroiTObject(lisParamBloco);


	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::Executa(void) {
	TLisDefeitoRede *pDefeitoRede;
	strResRede *resGeral, *resRede;

	// VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// AnsiString arq = path->DirTmp() + "\\parDefeitossimula.txt";
	//ImprimeManobrasCjtoDefeitos(NULL, arq);
//   paramConf->ImprimeParametrosSimula(arq);
//   ImprimeParametrosEntrada("paramEntradaConf.txt");

	//lisChavesAtivas: Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
	PreparaLisChavesAtivas();

   //hk201710IF //hk201808: reformula
   buscaProt->PreparaIndicacaoFalta();

	//Reinicia zonas de religamento caso hajam novos religadores ou alteracao da profundidade
	//buscaProt->ReiniciaZonaReliga();
   ////Reinicia todas as variáveis dos defeitos sem destruir listas e defeitos
	ReinciaLisDefeitos();
	//Gera blocos associados a defeito
   //pdef->LisBlocosAgrega
	GeraLisBlocosDefeito();

	//Inicia resultados de continuidade
   //Zera resultados geral e por rede (resGeral e lisResRede->resRede)
   resCont->IniciaResultados();
   //Verifica se as folgas de transferencia já foram avaliadas
   if((paramConf->ParamGerais->flagRecurso)&&(!flagVisAvalia))
	{
		AvaliaFolgaVis();
	}

   //ImprimeParametrosEntrada("ParamEntrada.txt");
   //ImprimeParametrosDefeito("ParamDefeito.txt");

   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      CalculaIndicadoresRedeMT(pDefeitoRede);
      CalculaIndicadoresRedeBT1(pDefeitoRede->rede);

      //ImprimeManobrasCjtoDefeitos(pDefeitoRede, arq, true);

      //Atualiza indicadores em resCont->LisResBloco
      //Atualiza indicadores em resCont->LisResBloco->resBloco
      //em funcao de pdef
      //onde pdef->bloco = resBloco->bloco
      //Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
      //resBloco->dcons    = pdef->DCons;    //desliga x cons     (deslig x consumidores)
      //resBloco->tcons    = pdef->TCons;    //tdesligax cons     (min x consumidores)
      //resBloco->nmanAuto = pdef->NmanAuto; //piscadas x nons    (piscadas x consumidores)
      //resBloco->nmanMan  = pdef->NmanMan;  //piscadas x consVIP (piscadas x consumidores)
      ExecutaAtualizaResBloco(pDefeitoRede);
      }

   //Executa acréscimo de indicadores devido a falhas em trafos de SE
   //Tarefas
     //1. banco e interface de dados
     //      a- acrescentar nova tabela a banco de dados, codTrafo, Taxa, trep, SobreCarga
     //      b- acrescentar interface
     //2. Execução, classe nova
     //      a- Preparação objeto por trafo de se, com parametros do banco
     //      a1. Circuitos por trafo
     //      a2. Curva de carga por trafo
     //      a3. Lista de trafos de socorro
     //      b- Execucao para cadas trafo i e patamar k (considerando distribuição uniforme de falhas no dia)
     //     b1. Existencia de possibilidade de transferencia para um dos trafos de transferencia
     //     b2. Calcular número de desligamentos e tempo de desligamento
     //3. Resultados: acrescentar resultados por SE, por Circuito e Geral
     //      a- Adicionar clientes e clientes hora por circuito

   //hk201804: redes pai-filha
   ExecutaAtualizaResRede();

   //Gera indicadores de resGeral
   resGeral = resCont->ResGeral;
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      resRede = resCont->ResRede[pDefeitoRede->rede];
		if (resRede == NULL)
			continue;

      resGeral->tdeslN     += resRede->tdeslN;
      resGeral->tdeslENDN  += resRede->tdeslENDN;
      resGeral->ndeslN     += resRede->ndeslN;
      resGeral->ndeslENDN  += resRede->ndeslENDN;
      resGeral->end        += resRede->end;
      resGeral->ndeslPisca    += resRede->ndeslPisca;
      resGeral->ndeslPiscaVIP += resRede->ndeslPiscaVIP;
      resGeral->tdeslBTN   += resRede->tdeslBTN;
      resGeral->ndeslBTN   += resRede->ndeslBTN;
      resGeral->endBT      += resRede->endBT;
      }

//   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
//   AnsiString arq;
//   arq = path->DirTmp() + "\\resCarga.txt";
//   resCont->ImprimeResCarga(arq);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ExecutaAtualizaResRede(TLisDefeitoRede *pDefeitoRedeAux)   //hk201804: redes pai-filha
   {
   TLisDefeitoRede *pDefeitoRede;
   strResRede      *resGeral, *resRede;
   strResBloco     *resBloco;
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      if(pDefeitoRedeAux != NULL) {if(pDefeitoRede != pDefeitoRedeAux) continue;}
      //soma indicadores por rede
      if((resRede  = resCont->ResRede[pDefeitoRede->rede]) == NULL) continue;

      for(int nb = 0; nb < resCont->LisResBloco->Count; nb++)
         {
		 resBloco = (strResBloco*)resCont->LisResBloco->Items[nb];
         //hk201701: problemas em redes AT : Acréscimo de variável resBloco->rede: if(resBloco->bloco->Rede != rede) continue;
			if (resBloco->rede != pDefeitoRede->rede)
				continue;
         resRede->tdeslN += (resBloco->tdesl * resBloco->ncons);
		 resRede->ndeslN += (resBloco->ndesl * resBloco->ncons);
         resRede->end           += resBloco->end;
         resRede->ndeslPisca    += (resBloco->npiscadas * resBloco->ncons);
		 resRede->ndeslPiscaVIP += (resBloco->npiscadas * resBloco->nconsVIP);
         resRede->tdeslENDN     += (resBloco->tdeslEND *  resBloco->ncons);
         resRede->ndeslENDN     += (resBloco->ndeslEND *  resBloco->ncons);
         }

      }

   }
//---------------------------------------------------------------------------
//hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
bool __fastcall TCalcIndCont::ExecutaRede(VTRede *rede, bool flagReiniciaRes)
   {
   TLisDefeitoRede *pDefeitoRede;

      //Atualiza resultados por blocos devido a todos os defeitos na rede:
      //     resBloco = resCont->ResBloco[blCalc];
      //     resBloco->tdesl    //tempos e desligamentos para desligamentos > 3min
      //     resBloco->ndesl
      //     resBloco->tdeslEND //tempos e desligamentos totais sem limitantes
      //     resBloco->ndeslEND
      //     resBloco->end      //end e energia não distribuida
      //Atualiza resultados por rede
      //   resRede = resCont->ResRede[rede];
      //     resRede->tdeslN += (resBloco->tdesl * resBloco->ncons);
      //     resRede->ndeslN += (resBloco->ndesl * resBloco->ncons);
      //     resRede->end   += resBloco->end;
      //     resRede->tdeslENDN += (resBloco->tdeslEND * resBloco->ncons);
      //     resRede->ndeslENDN += (resBloco->ndeslEND * resBloco->ncons);
      //Atualiza resultados para ajuste
      //   paramRede = ParametroConf(rede)) != NULL)
      //   paramRede->cliAj_mt = resRede->ndeslN;
      //     paramRede->clihAj_mt= resRede->tdeslN/60.;
   strResRede *resGeral, *resRede;
   if(rede == NULL)
     {
     resGeral = resCont->ResGeral;
     resGeral->tdeslN   = 0.0;
     resGeral->tdeslENDN= 0.0;
     resGeral->ndeslN   = 0.0;
     resGeral->ndeslENDN= 0.0;
     resGeral->end      = 0.0;
     resGeral->ndeslPisca    = 0.0;
     resGeral->ndeslPiscaVIP = 0.0;
     //resGeral->ncons    = 0;
     resGeral->tdeslBTN   = 0.0;
     resGeral->ndeslBTN   = 0.0;
     resGeral->endBT      = 0.0;
     return(true);
     }

   //lisChavesAtivas: Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
   PreparaLisChavesAtivas();
   //Reinicia zonas de religamento caso hajam novos religadores ou alteracao da profundidade
   //buscaProt->ReiniciaZonaReliga();

   //hk201710IF //hk201808: reformula
   buscaProt->PreparaIndicacaoFalta();

   ////Reinicia todas as variáveis dos defeitos sem destruir listas e defeitos
   ReinciaLisDefeitos(rede);
   //Gera blocos associados a defeito para considerar chaves fora de lischavesAtivas
   //pdef->LisBlocosAgrega
   GeraLisBlocosDefeito(rede);
   //Inicia resultados de continuidade
   //hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
	if (flagReiniciaRes == true)
		resCont->IniciaResultados(rede);
   //Verifica se as folgas de transferencia já foram avaliadas
	if ((paramConf->ParamGerais->flagRecurso) && (!flagVisAvalia))
	{
		AvaliaFolgaVis();
	}

   pDefeitoRede = NULL;
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
		if (pDefeitoRede->rede == rede)
			break;
      pDefeitoRede = NULL;
      }

    if(pDefeitoRede != NULL)
	{
        CalculaIndicadoresRedeMT(pDefeitoRede);

        //hkPC CalculaIndicadoresRedeBT(rede);
        CalculaIndicadoresRedeBT1(pDefeitoRede->rede);
    }
    else
        return (false);

   //Atualiza indicadores em resCont->LisResBloco
   //Atualiza indicadores em resCont->LisResBloco->resBloco
   //em funcao de pdef
   //onde pdef->bloco = resBloco->bloco
   //Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
   //resBloco->dcons    = pdef->DCons;    //desliga x cons     (deslig x consumidores)
   //resBloco->tcons    = pdef->TCons;    //tdesligax cons     (min x consumidores)
   //resBloco->nmanAuto = pdef->NmanAuto; //piscadas x nons    (piscadas x consumidores)
   //resBloco->nmanMan  = pdef->NmanMan;  //piscadas x consVIP (piscadas x consumidores)
   ExecutaAtualizaResBloco(pDefeitoRede);

   //hk201804: redes pai-filha
   //Efeito dos indicadores por bloco nas várias redes
   ExecutaAtualizaResRede(pDefeitoRede);

   //Atualiza resCont->ResGeral
   resGeral = resCont->ResGeral;
   resGeral->tdeslN   = 0.0;
   resGeral->tdeslENDN= 0.0;
   resGeral->ndeslN   = 0.0;
   resGeral->ndeslENDN= 0.0;
   resGeral->end      = 0.0;
   resGeral->ndeslPisca    = 0.0;
   resGeral->ndeslPiscaVIP = 0.0;
   //resGeral->ncons    = 0;
   resGeral->tdeslBTN   = 0.0;
   resGeral->ndeslBTN   = 0.0;
   resGeral->endBT      = 0.0;

   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      resRede = resCont->ResRede[pDefeitoRede->rede];
		if (resRede == NULL)
			continue;

      resGeral->tdeslN     += resRede->tdeslN;
      resGeral->tdeslENDN  += resRede->tdeslENDN;
      resGeral->ndeslN     += resRede->ndeslN;
      resGeral->ndeslENDN  += resRede->ndeslENDN;
      resGeral->end        += resRede->end;
      resGeral->ndeslPisca    += resRede->ndeslPisca;
      resGeral->ndeslPiscaVIP += resRede->ndeslPiscaVIP;
      resGeral->tdeslBTN   += resRede->tdeslBTN;
      resGeral->ndeslBTN   += resRede->ndeslBTN;
      resGeral->endBT      += resRede->endBT;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaDicFicPuMensal(void)
   {
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede  *rede;
   TList   *lisRede;
   int     icontaMes;

   //Executa o calculo mensal de indicadores e guarda por carga DIC_PU[12] e FIC_pu[12]
   //1- gera valores de taxa de falha e tempo de reparo em pu da média anual
   //Gera distribuição de falhas e tempos por mes
         //paramGeral->lamb_pu[nm] ,tr_pu[nm]
         //paramRede->lamb_pu[nm]  ,tr_pu[nm]
         //paramBloco->lamb_pu[nm] ,tr_pu[nm]
         //paramCarga->lamb_pu[nm] ,tr_pu[nm]
   //função de
   //paramEqpto->nFEquip[nm];
   //paramEqpto->climes[nm], clihmes[nm]
   //e
   //paramGeral->lamb_pu[nm]
   //paramGeral->tr_pu[nm]
   //funcao de
   //paramGeral->nfEquip e paramGeral->durTotal
   icontaMes = geraLisParamEqpto->RedistribuiFalhasCircMes(paramConf);
   //resCont->lisResCarga->resCarga->dic_pu[0-11], fic_pu[0-11] = 0.0
   resCont->IniciaDicFicPu();
   //Limite em 12 meses de ocorrências ao longo do ano
	if (icontaMes < 12)
		return (false);

   //lisChavesAtivas: Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
   PreparaLisChavesAtivas();
   //hk201710IF //hk201808: reformula
   buscaProt->PreparaIndicacaoFalta();

//   AnsiString aux;
//   double    daux;
   //Executa calculo mensal para todos os meses
   for(int nm = 0; nm < 12; nm++)
		{
      ExecutaCalculoMensal(nm);
//      daux = resCont->ResGeral->tdeslN / 60/ (double)resCont->ResGeral->ncons;
//      aux = "DEC = ";
//      aux += DoubleToStr("%6.2f", daux);
//      daux = resCont->ResGeral->ndeslN / (double)resCont->ResGeral->ncons;
//      aux +="/FEC = ";
//      aux += DoubleToStr("%6.2f", daux);
//      Aviso(aux);
      }
   //Gera valores de dic e fic em pu
   resCont->IniciaDicFicPu(false);

   return(true);

   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaCalculoMensal(int iMes)
   {
   TLisDefeitoRede *pDefeitoRede;
   strResRede      *resGeral, *resRede;

   //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda * lambda_pu[iMes];
   //pdef->TReparo           = paramBloco->trep * tr_pu[iMes];
   //--->pdef->TaxaFalha     = paramRede->lambda * lambda_pu[iMes];
   //--->pdef->TReparo       = paramRede->trep * tr_pu[iMes];
   //--->--->pdef->TaxaFalha = paramDefault->lambda * lambda_pu[iMes];
   //--->--->pdef->TReparo   = paramDefault->trep * tr_pu[iMes];
   IniciaParamDefeitoMes(iMes);
   //Reinicia zonas de religamento caso hajam novos religadores ou alteracao da profundidade
   //buscaProt->ReiniciaZonaReliga();
   ////Reinicia todas as variáveis dos defeitos sem destruir listas e defeitos
   ReinciaLisDefeitos();
   //Gera blocos associados a defeito
   //pdef->LisBlocosAgrega
   GeraLisBlocosDefeito();

   //Inicia resultados de continuidade
   //Zera resultados geral e por rede (resGeral e lisResRede->resRede, resBloco e resCarga)
   resCont->IniciaResultados();
   //Verifica se as folgas de transferencia já foram avaliadas
   if((paramConf->ParamGerais->flagRecurso)&&(!flagVisAvalia))
	{
		AvaliaFolgaVis();
	}

   //ImprimeParametrosEntrada("ParamEntrada.txt");
   //ImprimeParametrosDefeito("ParamDefeito.txt");
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];

      //resBloco->ndesl    //tempos e desligamentos (sem multiplicar por ncons) para desligamentos > 3min
      //resBloco->tdesl
      //resBloco->ndeslEND //tempos e desligamentos  (sem multiplicar pors ncons) totais sem limitantes
      //resBloco->tdeslEND
      //resBloco->end      //end e energia não distribuida
      CalculaIndicadoresRedeMT(pDefeitoRede);
      //Calcula indicadores e guarda resultados, resCont por carga
      CalculaIndicadoresRedeBT1(pDefeitoRede->rede, iMes);
      //Atualiza indicadores em resCont->LisResBloco
      //Atualiza indicadores em resCont->LisResBloco->resBloco
      //em funcao de pdef
      //onde pdef->bloco = resBloco->bloco
      //Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
      //resBloco->dcons    = pdef->DCons;    //desliga x cons     (deslig x consumidores)
      //resBloco->tcons    = pdef->TCons;    //tdesligax cons     (min x consumidores)
      //resBloco->nmanAuto = pdef->NmanAuto; //piscadas x nons    (piscadas x consumidores)
      //resBloco->nmanMan  = pdef->NmanMan;  //piscadas x consVIP (piscadas x consumidores)
      ExecutaAtualizaResBloco(pDefeitoRede);
      }

   //Gera indicadores de resGeral
   resGeral = resCont->ResGeral;
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      resRede = resCont->ResRede[pDefeitoRede->rede];
		if (resRede == NULL)
			continue;

      resGeral->tdeslN     += resRede->tdeslN;
      resGeral->tdeslENDN  += resRede->tdeslENDN;
      resGeral->ndeslN     += resRede->ndeslN;
      resGeral->ndeslENDN  += resRede->ndeslENDN;
      resGeral->end        += resRede->end;
      resGeral->ndeslPisca    += resRede->ndeslPisca;
      resGeral->ndeslPiscaVIP += resRede->ndeslPiscaVIP;
      resGeral->tdeslBTN   += resRede->tdeslBTN;
      resGeral->ndeslBTN   += resRede->ndeslBTN;
      resGeral->endBT      += resRede->endBT;
      }
   //Reinicia param Defeito com valores anuais
   IniciaParamDefeito();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ExecutaAtualizaResBloco(TLisDefeitoRede *pDefeitoRede)
   {
   //Atualiza indicadores em resCont->LisResBloco
   //Atualiza indicadores em resCont->LisResBloco->resBloco
   //em funcao de pdef
   //onde pdef->bloco = resBloco->bloco
   //Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
   //resBloco->dcons    = pdef->DCons;    //desliga x cons     (deslig x consumidores)
   //resBloco->tcons    = pdef->TCons;    //tdesligax cons     (min x consumidores)
   //resBloco->nmanAuto = pdef->NmanAuto; //piscadas x nons    (piscadas x consumidores)
   //resBloco->nmanMan  = pdef->NmanMan;  //piscadas x consVIP (piscadas x consumidores)
   TList *lisAux = new TList();
   strResBloco *resBloco;
   VTDefeito   *pdef;
   lisAux->Assign(resCont->LisResBloco, laOr);
   for(int n = lisAux->Count - 1; n >=0; n--)
      {
      //hk201701: problemas em redes AT : blocos com apenas 1 barra atribui rede de chave de fornecimento : Acréscimo de variável resBloco->rede
      //if((resBloco = (strResBloco*)lisAux->Items[n])->bloco->Rede == pDefeitoRede->rede) continue;
		if ((resBloco = (strResBloco*)lisAux->Items[n])->rede == pDefeitoRede->rede)
			continue;
      lisAux->Delete(n);
      }
   for(int ndef = 0; ndef < pDefeitoRede->lisDefeitos->Count; ndef++)
      {
      pdef = (VTDefeito*)pDefeitoRede->lisDefeitos->Items[ndef];
      resBloco = NULL;
      for(int n = 0; n < lisAux->Count; n++)
         {
         if((resBloco = (strResBloco*)lisAux->Items[n])->bloco == pdef->Bloco)
			{
				lisAux->Delete(n);
             break;
            }
         resBloco = NULL;
         }
      if(resBloco != NULL)
        {
        resBloco->dcons    = pdef->DCons;
        resBloco->tcons    = pdef->TCons;
        resBloco->nmanAuto = pdef->NmanAuto;
        resBloco->nmanMan  = pdef->NmanMan;
        }
      }
   delete lisAux;
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ExecutaAjusteLisParamVerif(void)
{
    TList        *lisVerifCjto = paramConf->LisVerifCjto;
    VTRedes      *redes        = (VTRedes*) apl->GetObject(__classid(VTRedes));
    VTRede       *rede;
    TList        *lisSE        = radial->LisSubestacao();
    VTVerifCjto  *verifCjto;
    VTVerifSe    *verifSe;
    VTSubestacao *se;
    TList        *lisPrimario;
    TList        *lisRede      = redes->LisRede();
    AnsiString    aux;

    bool flagCalcGeral = false;
    bool flagSeNe; //se encontrou se ou circuito

    double decVerif, fecVerif, decAjust, fecAjust;
    TList *lisAux = new TList;

    unsigned short year, month, day, hour, min, sec, msec;
    AnsiString dataHoraLog;
    TDateTime hhh;

    hhh = hhh.CurrentDateTime();
    hhh.DecodeDate(&year, &month, &day);
    hhh.DecodeTime(&hour, &min, &sec, &msec);
    dataHoraLog  = hhh.DateString();
    dataHoraLog += "   ";
    dataHoraLog += hhh.TimeString();
    aux.sprintf("%s: Início do Ajuste de Parâmetros", dataHoraLog.c_str());
    log->Add(aux);

    //Executa ajuste por conjunto
    for(int nv = 0; nv < lisVerifCjto->Count; nv++)
    {
        verifCjto = (VTVerifCjto*)lisVerifCjto->Items[nv];

        decVerif  = verifCjto->DecVerif;
        fecVerif  = verifCjto->FecVerif;
        if((!(decVerif > 0.0)) || (!(fecVerif > 0.0)))
            continue;

        lisAux->Clear();
        flagSeNe = false;
        for(int nse = 0; nse < verifCjto->LisVerifSe->Count; nse++)
        {
            verifSe = (VTVerifSe*)verifCjto->LisVerifSe->Items[nse];
            //Verifica se já houve ajuste
//          if((decVerif > 0.0)&&(fecVerif > 0.0)) continue;
            se = NULL;
            for(int nse = 0; nse < lisSE->Count; nse++)
            {
                se = (VTSubestacao*)lisSE->Items[nse];
                if(se->Rede->Codigo.AnsiCompare(verifSe->Codigo) == 0)
                    break;
                se = NULL;
            }
            if(se == NULL)
            {
                //Busca diretamente os circuitos
                rede = NULL;
                for(int nr = 0; nr < lisRede->Count; nr++)
                {
                    rede = (VTRede*)lisRede->Items[nr];
                    if(rede->Codigo.AnsiCompare(verifSe->Codigo) == 0)
                        break;
                    rede = NULL;
                }
                if (rede != NULL)
                    lisAux->Add(rede);
                else
                    flagSeNe = true; //se e circuito não encontrados
            }
            else
            {
                //Obtem a lista de circuitos
                lisPrimario = se->LisPrimario();
                if (lisPrimario->Count == 0)
                    continue;
                for(int npr = 0; npr < lisPrimario->Count; npr++)
                {
                    VTPrimario *primario = (VTPrimario*)lisPrimario->Items[npr];
                    lisAux->Add(primario->Rede);
                }
            }
            //Verifica se não encontrou Se nem Circuito
            if(flagSeNe == true)
            {
                aux.sprintf("Conjunto não Ajustado: %s", verifCjto->Codigo.c_str());
                log->Add(aux);
                aux.sprintf("SE/Circuito não encontrado: %s", verifSe->Codigo.c_str());
                log->Add(aux);
                break;
            }
        }

        if((lisAux->Count > 0)&&(flagSeNe == false))
        {
            ExecutaAjusteParametrosLisRedeBlocoCarga(lisAux, decVerif, fecVerif, decAjust, fecAjust);//hk201802a: ExecutaAjusteParametrosLisRede(lisAux, decVerif, fecVerif, decAjust, fecAjust);
            verifCjto->DecAjust = decAjust;
            verifCjto->FecAjust = fecAjust;
        }
    }

    // Executa ajusta por SE
    for(int nv = 0; nv < lisVerifCjto->Count; nv++)
    {
        verifCjto = (VTVerifCjto*)lisVerifCjto->Items[nv];
        for(int nse = 0; nse < verifCjto->LisVerifSe->Count; nse++)
        {
            verifSe = (VTVerifSe*)verifCjto->LisVerifSe->Items[nse];
            decVerif  = verifSe->DecVerif;
            fecVerif  = verifSe->FecVerif;
            if ((!(decVerif > 0.0)) || (!(fecVerif > 0.0)))
                continue;

            se = NULL;
            for(int nse = 0; nse < lisSE->Count; nse++)
            {
                se = (VTSubestacao*)lisSE->Items[nse];
                if (se->Rede->Codigo.AnsiCompare(verifSe->Codigo) == 0)
                    break;
                se = NULL;
            }
            if (se == NULL)
                continue;
            lisPrimario = se->LisPrimario();
            if (lisPrimario->Count == 0)
                continue;
            lisAux->Clear();
            for(int npr = 0; npr < lisPrimario->Count; npr++)
            {
                VTPrimario *primario = (VTPrimario*)lisPrimario->Items[npr];
                lisAux->Add(primario->Rede);
            }
            ExecutaAjusteParametrosLisRedeBlocoCarga(lisAux, decVerif, fecVerif, decAjust, fecAjust);//hk201802a: ExecutaAjusteParametrosLisRede(lisAux, decVerif, fecVerif, decAjust, fecAjust);
            verifSe->DecAjust = decAjust;
            verifSe->FecAjust = fecAjust;
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    //Caso existam subestacoes sem valores verificados realiza o cálculo geral //
    //Executa para toda a rede                                                 //
    /////////////////////////////////////////////////////////////////////////////
    //Reinicia lisParamBloco->paramBloco->(lambda, trep)
    //Funcao de:
    //paramEqpto->lambda
    //paramEqpto->trep
    //e distribuição de equipamentos de protecao
    //hk201802a: geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
    // hkmar (paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);

    //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
    //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
    //Atualiza taxas (por blocos-->por circuito-->valor geral
    //pdef->TaxaFalha         = paramBloco->lambda;
    //pdef->TReparo           = paramBloco->trep;
    //--->pdef->TaxaFalha     = paramRede->lambda;
    //--->pdef->TReparo       = paramRede->trep;
    //--->--->pdef->TaxaFalha = paramDefault->lambda;
    //--->--->pdef->TReparo   = paramDefault->trep;
    IniciaParamDefeito();

    //Executa o calculo dos indicadores de continuidade
    Executa();

    //delete radial;
    delete lisAux;
}

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaAjusteParametrosLisRede(TList *lisRede, double decMeta,
	double fecMeta, double &decAjusta, double &fecAjusta, double erroMax, int iteraMax)      //SAI201805
   {
   TList *lisParamRede  = paramConf->LisParamRede;
   TList *lisParamEqpto = paramConf->LisParamEqpto;
   VTRede  *rede;
   VTCarga *carga;
   TList   *lisParamEqptoMT, *lisParamEqptoBT, *lisCargas;
   double erroAtual, valorMeta, valorAjustado, fatAjuste, erro;
   strDadoParamRede *paramRede;
   strDadoParamBloco *paramBloco;
   strDadoParamEqpto *paramEqpto;
   strResRede   *resRede;
   int    iconta;
   int    ncons;
   bool   resposta = true;

	if (IsDoubleZero(decMeta) || IsDoubleZero(fecMeta))
		return (false);
	if (lisRede->Count == 0)
		return (false);
   //Inicia lisParamRede com parametros com valores default -- saiu já inicializado em ParamDefault
   //Calcula o número de consumidores de lisRede
   ncons = 0;
   for(int nr = 0; nr < lisRede->Count; nr++)
      {
      rede = (VTRede*)lisRede->Items[nr];
      //Protecao
		if (!rede->Carregada)
			continue;
      //Calcula o total de consumidores de lisRedes
      resRede = resCont->ResRede[rede];
      ncons += resRede->ncons;
      }

	if (ncons == 0)
		return (false);
   //Gera listas de paramEqpto para MT e BT para as listas de rede
   lisParamEqptoMT = new TList();
   lisParamEqptoBT = new TList();
   for(int ne = 0; ne < lisParamEqpto->Count; ne++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[ne];
		if (paramEqpto->rede == NULL)
			continue;
		if (lisRede->IndexOf(paramEqpto->rede) == -1)
			continue;
      if(paramEqpto->eqpto != NULL)
          lisParamEqptoMT->Add(paramEqpto);
      else
        {
        if(paramEqpto->carga != NULL)
          lisParamEqptoBT->Add(paramEqpto);
        }
      }

   //Reinicia lisParamBloco->paramBloco->(lambda, trep)
   //Funcao de:
   //paramEqpto->lambda
   //paramEqpto->trep
	geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
	// hkmar (paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);

   //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramDefault->lambda;
   //--->--->pdef->TReparo   = paramDefault->trep;
   IniciaParamDefeito();
   //Ajuste de FEC da lista de redes
   //this->ResRede[rede];
   //Ajuste de CLI

   valorMeta = fecMeta * ncons;//paramRede->cli_mt;
   iconta = 0;
   erroAtual = 100.0;
   while((erroAtual > erroMax)&&(iconta < iteraMax))
           {
           //Atualiza resultados por blocos devido a todos os defeitos na rede:
           //     resBloco = resCont->ResBloco[blCalc];
           //Atualiza resultados por rede
           //   resRede = resCont->ResRede[rede];
           //Atualiza resultados para ajuste
           //   paramRede = ParametroConf(rede)) != NULL)
           //   paramRede->cliAj_mt = resRede->ndeslN;
           //    paramRede->clihAj_mt= resRede->tdeslN/60.;
           valorAjustado = 0;
           resCont->IniciaResultados(); //hk201701: Adaptação redes subtransmissao: flagIniciaRes
            for(int nr = 0; nr < lisRede->Count; nr++)
            {
                rede = (VTRede*)lisRede->Items[nr];
                //Protecao
                if (!rede->Carregada)
                    continue;
                ExecutaRede(rede, false);
                // hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
                resRede   = resCont->ResRede[rede];
                valorAjustado += resRede->ndeslN;
            }
//            double daux = valorAjustado/ncons;
            //Protecao
            if (IsDoubleZero(valorAjustado))
                fatAjuste = 1.0;
            else
                fatAjuste = valorMeta / valorAjustado;
           iconta++;
           erroAtual = fabs(1.0 - 1.0/fatAjuste);
           if(erroAtual > erroMax)
              {
              if(flagParBloco)
                {
                //Atualiza taxas por equipamento
                for(int ne = 0; ne < lisParamEqptoMT->Count; ne++)
                   {
                   paramEqpto = (strDadoParamEqpto*)lisParamEqptoMT->Items[ne];
                   paramEqpto->lambda       *= fatAjuste;
                   //hkmuda ajuste paramEqpto->nFalhasEquip *= fatAjuste;
                   }
                for(int ne = 0; ne < lisParamEqptoBT->Count; ne++)
                   {
                   paramEqpto = (strDadoParamEqpto*)lisParamEqptoBT->Items[ne];
                   paramEqpto->lambda  *= fatAjuste;
                   //hkmuda ajuste paramEqpto->nFalhasEquip *= fatAjuste;
                   }
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
                   paramRede->lambda *= fatAjuste;         //muda ajuste
                   }
                }
              else //atualiza somente taxas por rede
                {
				    for (int nr = 0; nr < lisRede->Count; nr++)
				       {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
                   paramRede->lambda *= fatAjuste;
                   }
                }
              //Reinicia lisParamBloco->paramBloco->(lambda, trep)
              //Funcao de:
              //paramEqpto->lambda
              //paramEqpto->trep
			     geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
			     // hkmar (paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);
              //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
              IniciaParamDefeito();
              }
           }
	  if(iconta == iteraMax)
	    resposta = false;
     fecAjusta = valorAjustado / ncons;
//     erro = erroAtual;
     //Ajuste de DEC
     //Ajuste de CLIh
     valorMeta = decMeta * ncons * 60;//paramRede->clih_mt * 60;
     iconta = 0;
     erroAtual = 100.0;
     while((erroAtual > erroMax)&&(iconta < iteraMax))
        {
            valorAjustado = 0;
            resCont->IniciaResultados(); //hk201701: Adaptação redes subtransmissao: flagIniciaRes
            for(int nr = 0; nr < lisRede->Count; nr++)
            {
                rede = (VTRede*)lisRede->Items[nr];
                //Protecao
                if (!rede->Carregada)
                    continue;
                ExecutaRede(rede, false);
                // hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
                resRede   = resCont->ResRede[rede];
                valorAjustado += resRede->tdeslN;
            }

            //Protecao
            if (IsDoubleZero(valorAjustado))
                fatAjuste = 1.0;
            else
                fatAjuste = valorMeta / valorAjustado;

           iconta++;
           erroAtual = fabs(1.0 - 1.0/fatAjuste);
           if(erroAtual > erroMax)
                {
			       if (flagParBloco)
			       {
                //Atualiza taxas por equipamento
                for(int ne = 0; ne < lisParamEqptoMT->Count; ne++)
                   {
                   paramEqpto = (strDadoParamEqpto*)lisParamEqptoMT->Items[ne];
					    paramEqpto->trep = paramEqpto->trep * fatAjuste;
					    // (paramEqpto->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   //hkmuda ajuste paramEqpto->trep = 0.0;                            //muda ajuste
                   //hkmuda ajuste if(paramEqpto->nFalhasEquip > 0) paramEqpto->trep = paramEqpto->durTotal / paramEqpto->nFalhasEquip;
                   //hkmuda ajuste paramEqpto->trep = paramEqpto->trep * fatAjuste;//(paramEqpto->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   //hkmuda ajuste paramEqpto->durTotal = paramEqpto->trep * paramEqpto->nFalhasEquip;

                   }

                //Atualiza taxas por equipamento
                for(int ne = 0; ne < lisParamEqptoBT->Count; ne++)
                   {
                   paramEqpto = (strDadoParamEqpto*)lisParamEqptoBT->Items[ne];
                   paramEqpto->trep *= fatAjuste; //muda ajuste
                   //hkmuda paramEqpto->trep *= fatAjuste;
                   //hkmuda paramEqpto->durTotal = paramEqpto->trep * paramEqpto->nFalhasEquip;
                   }

                  //Guarda valores ajustados
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
					    paramRede->trep = paramRede->trep * fatAjuste;
					    // (paramRede->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   }
                }
              else
                {
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
					    paramRede->trep = paramRede->trep * fatAjuste;
					    // (paramRede->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   }
                }

             //Reinicia lisParamBloco->paramBloco->(lambda, trep)
             //Funcao de:
             //paramEqpto->lambda
             //paramEqpto->trep
			geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
			// hkmar paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);
             //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
             IniciaParamDefeito();
             }
           }
//	if (erroAtual > erro)
//		erro = erroAtual; // hk201701: correcao de atualização de erro
	if (iconta == iteraMax)
		resposta = false;
     decAjusta = valorAjustado / ncons/ 60.0;

     //}
   //else
     //{
     //ExecutaRede(rede);
     //}
   /*if(resposta)
     {
     //Guarda valores ajustados
     for(int nr = 0; nr < lisRede->Count; nr++)
        {
        rede = (VTRede*)lisRede->Items[nr];
        //Protecao
        if(!rede->Carregada) continue;
        resRede   = resCont->ResRede[rede];
        paramRede = ParametroConf(rede);
        paramRede->cliAj_mt = resRede->ndeslN;
        paramRede->clihAj_mt= resRede->tdeslN/60.;
        //hkpc paramRede->flagAjusta = true;
        }
    }
   */
   delete lisParamEqptoMT;
   delete lisParamEqptoBT;
   return(resposta);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ExecutaAjusteParametrosLisRedeBlocoCarga(TList *lisRede,
	double decMeta, double fecMeta, double &decAjusta, double &fecAjusta, double erroMax,
	int iteraMax)
   {
   //Ajuste direto (e nao indireto através de paramEqpto->lambda, paramEqpto->trep)
   //de lisParamRede, lisParamBloco e lisParamCarga

   TList *lisParamRede = paramConf->LisParamRede;
   TList *lisParamBloco= paramConf->LisParamBloco;
   TList *lisParamCarga= paramConf->LisParamCarga;
   VTRede  *rede;
   VTCarga *carga;
   TList   *lisParamBlocoRedes, *lisParamCargaRedes;
   double erroAtual, valorMeta, valorAjustado, fatAjuste, erro;
   strDadoParamRede  *paramRede;
   strDadoParamBloco *paramBloco;
   strDadoParamCarga *paramCarga;
   strResRede    *resRede;
   int    iconta;
   int    ncons;
   bool   resposta = true;

	if (IsDoubleZero(decMeta) || IsDoubleZero(fecMeta))
		return (false);
	if (lisRede->Count == 0)
		return (false);

    //Calcula o número de consumidores de lisRede
    ncons = 0;
    for(int nr = 0; nr < lisRede->Count; nr++)
    {
        rede = (VTRede*)lisRede->Items[nr];
        //Protecao
        if (!rede->Carregada)
            continue;
        //Calcula o total de consumidores de lisRedes
        resRede = resCont->ResRede[rede];
        ncons += resRede->ncons;
    }

	if (ncons == 0)
		return (false);

   //Prepara listas de parametros por bloco e carga para serem ajustados
   lisParamBlocoRedes = new TList();
   lisParamCargaRedes = new TList();
   lisParamBlocoRedes->Assign(lisParamBloco, laOr);
   lisParamCargaRedes->Assign(lisParamCarga, laOr);
   for(int n = lisParamBlocoRedes->Count  - 1; n >= 0; n--) //Verif201701
    {
        //gh201808: proteção para blocos definidos por chaves NA
        if (((strDadoParamBloco*)lisParamBlocoRedes->Items[n])->bloco == NULL)
            continue;

        if (lisRede->IndexOf(((strDadoParamBloco*)lisParamBlocoRedes->Items[n])->bloco->Rede) != -1)
            continue;

        lisParamBlocoRedes->Delete(n);
    }
    for(int n = lisParamCargaRedes->Count  - 1; n >= 0; n--) //Verif201701
    {
        if (lisRede->IndexOf(((strDadoParamCarga*)lisParamCargaRedes->Items[n])->rede) != -1)
            continue;
       lisParamCargaRedes->Delete(n);
    }

   //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramDefault->lambda;
   //--->--->pdef->TReparo   = paramDefault->trep;
   IniciaParamDefeito();
   //Ajuste de FEC da lista de redes
   //this->ResRede[rede];
   //Ajuste de CLI
   valorMeta = fecMeta * ncons;//paramRede->cli_mt;
   iconta = 0;
   erroAtual = 100.0;
   while((erroAtual > erroMax)&&(iconta < iteraMax))
           {
           //Atualiza resultados por blocos devido a todos os defeitos na rede:
           //     resBloco = resCont->ResBloco[blCalc];
           //Atualiza resultados por rede
           //   resRede = resCont->ResRede[rede];
           //Atualiza resultados para ajuste
           //   paramRede = ParametroConf(rede)) != NULL)
           //   paramRede->cliAj_mt = resRede->ndeslN;
           //    paramRede->clihAj_mt= resRede->tdeslN/60.;
           valorAjustado = 0;
           resCont->IniciaResultados(); //hk201701: Adaptação redes subtransmissao: flagIniciaRes
            for(int nr = 0; nr < lisRede->Count; nr++)
            {
                rede = (VTRede*)lisRede->Items[nr];
                //Protecao
                if (!rede->Carregada)
                    continue;
                ExecutaRede(rede, false);
                // hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
                resRede   = resCont->ResRede[rede];
                valorAjustado += resRede->ndeslN;
            }

            //Protecao
            if (IsDoubleZero(valorAjustado))
                fatAjuste = 1.0;
            else
                fatAjuste = valorMeta / valorAjustado;
           iconta++;
           erroAtual = fabs(1.0 - 1.0/fatAjuste);
           if(erroAtual > erroMax)
              {
                if(flagParBloco)
                {
                //Atualiza taxas por equipamento
                for(int ne = 0; ne < lisParamBlocoRedes->Count; ne++)
                   {
                   paramBloco = (strDadoParamBloco*)lisParamBlocoRedes->Items[ne];
                   paramBloco->lambda       *= fatAjuste;
                   //hkmuda ajuste paramEqpto->nFalhasEquip *= fatAjuste;
                   }
                for(int ne = 0; ne < lisParamCargaRedes->Count; ne++)
                   {
                   paramCarga = (strDadoParamCarga*)lisParamCargaRedes->Items[ne];
                   paramCarga->lambda  *= fatAjuste;
                   //hkmuda ajuste paramEqpto->nFalhasEquip *= fatAjuste;
                   }
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
                   paramRede->lambda *= fatAjuste;         //muda ajuste
                   }
                }
              else //atualiza somente taxas por rede
                {
				    for(int nr = 0; nr < lisRede->Count; nr++)
				       {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
                   paramRede->lambda *= fatAjuste;
                   }
                }
              //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
              IniciaParamDefeito();
              }
           }
	  if(iconta == iteraMax)
		 resposta = false;
     fecAjusta = valorAjustado / ncons;
//     erro = erroAtual;

     //Ajuste de DEC
     //Ajuste de CLIh
     valorMeta = decMeta * ncons * 60;//paramRede->clih_mt * 60;
     iconta = 0;
     erroAtual = 100.0;
     while((erroAtual > erroMax)&&(iconta < iteraMax))
           {
           valorAjustado = 0;
           resCont->IniciaResultados(); //hk201701: Adaptação redes subtransmissao: flagIniciaRes
           for(int nr = 0; nr < lisRede->Count; nr++)
              {
              rede = (VTRede*)lisRede->Items[nr];
              //Protecao
			     if (!rede->Carregada)
				  continue;
			     ExecutaRede(rede, false);
			     // hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
              resRede   = resCont->ResRede[rede];
              valorAjustado += resRede->tdeslN;
              }

           //Protecao
		     if (IsDoubleZero(valorAjustado))
			     fatAjuste = 1.0;
           else
              fatAjuste = valorMeta / valorAjustado;

           iconta++;
           erroAtual = fabs(1.0 - 1.0/fatAjuste);
                {
           if(erroAtual > erroMax)
			       if (flagParBloco)
			       {
                //Atualiza taxas por equipamento
                for(int ne = 0; ne < lisParamBlocoRedes->Count; ne++)
                   {
                   paramBloco = (strDadoParamBloco*)lisParamBlocoRedes->Items[ne];
					    paramBloco->trep = paramBloco->trep * fatAjuste;
					    // (paramEqpto->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   }
                for(int ne = 0; ne < lisParamCargaRedes->Count; ne++)
                   {
                   paramCarga = (strDadoParamCarga*)lisParamCargaRedes->Items[ne];
                   paramCarga->trep *= fatAjuste; //muda ajuste
                   //hkmuda ajuste paramEqpto->nFalhasEquip *= fatAjuste;
                   }

                  //Guarda valores ajustados
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
					paramRede->trep = paramRede->trep * fatAjuste;
					    // (paramRede->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   }
                }
              else
                {
                for(int nr = 0; nr < lisRede->Count; nr++)
                   {
                   rede = (VTRede*)lisRede->Items[nr];
                   //Protecao
					    if (!rede->Carregada)
						 continue;
                   //paramRede = ParametroConf(rede);
                   paramRede = paramConf->ParamRede[rede];
					paramRede->trep = paramRede->trep * fatAjuste;
					    // (paramRede->trep + this->TLocaliza) * fatAjuste - this->TLocaliza;
                   }
                }

             //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
             IniciaParamDefeito();
             }
           }
	if (iconta == iteraMax)
		resposta = false;
   decAjusta = valorAjustado / ncons/ 60.0;

   delete lisParamBlocoRedes;
   delete lisParamCargaRedes;
   return(resposta);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::GeraLisBlocosDefeito(VTRede *rede)
   {
   //Executada em
	//InsereLisChavesTelecom(TList *lisChavesTelecomExterno)
   //IniciaLisDefeitos(VTRede *rede)
   //Executa(void)
   //ExecutaRede(VTRede *rede)
   //ExecutaCalculoMensal(int iMes)
   VTDefeito *pdef;
   //Popula lista de blocos agregados por defeito = f(buscaProt->lisChavesAtivas)
   for(int nd = 0; nd < lisDefeitos->Count; nd++)
      {
      pdef = (VTDefeito*)lisDefeitos->Items[nd];
      if(rede != NULL)
		{
			if (pdef->Rede != rede)
				continue;
		}
      //hk201808: reformula sai
      //Gera lista de blocos do defeito pdef->LisBlocosAgrega
      //BuscaBlocosAgreg(pdef->Bloco, pdef->LisBlocosAgrega);

      //Gera para cada defeito os blocos agregados considerando chaves na rede mas nao ativadas
      /////////////////////////////////////////////////////////////////////////////////////////
      //Funcao que preenche lista com conjunto de blocos vizinhos a bloco ligados e com o//////
      //proprio bloco//////////////////////////////////////////////////////////////////////////
      //por chaves nao ativas//////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////
      buscaProt->BuscaBlocoAgreg(pdef->Bloco, pdef->LisBlocosAgrega);
      }

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::CalculaIndicadoresRedeMT(TLisDefeitoRede *pDefeitoRede)
   {
   TList       *lisBloco, *lisRede, *lisBlocosTot;
   VTBloco     *blocoDef;
   VTDefeito   *pdef;
   VTRede      *rede;
   strResRede  *resRede;//, *resGeral;
   strResBloco *resBloco;

   //Seta a rede de cálculo
   rede = pDefeitoRede->rede;
   for(int ndef = 0; ndef < pDefeitoRede->lisDefeitos->Count; ndef++)
      {
      pdef = (VTDefeito*)pDefeitoRede->lisDefeitos->Items[ndef];
      //Cálculo de:
      //Impacto de cada defeito:   pdef->DCons: desliga x cons
      //                           pdef->TCons: desliga x tdesliga x cons
      //                           pdef->NmanAuto: #manobras automáticas
      //                           pdef->NmanMan: #manobras manuais
      //Atualiza resultados devido a somatoria dos defeitos pdef:
      //Soma dos impactos
      //nos blocos:               resBloco = resCont->ResBloco[blCalc];
      //                           resBloco->ndesl    //tempos e desligamentos (sem multiplicar por ncons) para desligamentos > 3min
      //                           resBloco->tdesl
      //                           resBloco->ndeslEND //tempos e desligamentos  (sem multiplicar pors ncons) totais sem limitantes
      //                           resBloco->tdeslEND
      //                           resBloco->end      //end e energia não distribuida

	   CalculaIndicadoresDefeitoCenario(pdef);//hk201808: indisp: temp: CalculaIndicadoresDefeito(pdef);
      }

   /*//hk201804: redes pai-filha
   //soma indicadores por rede
   resRede  = resCont->ResRede[rede];
   //resGeral = resCont->ResGeral;


   //Zera indicadores relativos a falhas MT
   resRede->tdeslN    = 0;
   resRede->ndeslN    = 0;
   resRede->end       = 0;
   resRede->ndeslPisca= 0;
   resRede->ndeslPiscaVIP= 0;
   resRede->tdeslENDN = 0;
   resRede->ndeslENDN = 0;

   for(int nb = 0; nb < resCont->LisResBloco->Count; nb++)
      {
      resBloco = (strResBloco*)resCont->LisResBloco->Items[nb];
      //hk201701: problemas em redes AT : Acréscimo de variável resBloco->rede: if(resBloco->bloco->Rede != rede) continue;
		if (resBloco->rede != rede)
			continue;
      resRede->tdeslN += (resBloco->tdesl * resBloco->ncons);
      resRede->ndeslN += (resBloco->ndesl * resBloco->ncons);
      resRede->end           += resBloco->end;
      resRede->ndeslPisca    += (resBloco->npiscadas * resBloco->ncons);
      resRede->ndeslPiscaVIP += (resBloco->npiscadas * resBloco->nconsVIP);
      resRede->tdeslENDN     += (resBloco->tdeslEND *  resBloco->ncons);
      resRede->ndeslENDN     += (resBloco->ndeslEND *  resBloco->ncons);
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::CalculaIndicadoresRedeBT1(VTRede *rede, int iMes)
   {
   VTDemanda   *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   strDadoParamCarga *paramCarga;
   strResBloco *resBloco;
   strResCarga *resCarga;
   strResRede  *resRede;
   double      *distFalhasPat = paramConf->DistFalhasPat;
   TList       *lisResCarga;
   double      fatLambda, fatTrep;
   double      lambda, trep;
	double fatTMA;
	// hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por bloco

   //FILE  *fout;
   //VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   //AnsiString arq;
   //arq = path->DirTmp()+"\\CalculaIndicadoresRedeBT1.txt";
   //fout = fopen(arq.c_str(), "a");

   //Busca resRede
   resRede = resCont->ResRede[rede];
   resRede->tdeslBTN  = 0.0;
   resRede->ndeslBTN  = 0.0;
   resRede->endBT     = 0.0;

   for(int nb= 0; nb < resCont->LisResBloco->Count; nb++)
      {
      resBloco = (strResBloco*)resCont->LisResBloco->Items[nb];

      //hk201701: problemas em redes AT : Acréscimo de variável resBloco->rede: if(resBloco->bloco->Rede != rede) continue;
		if (resBloco->rede != rede)
			continue;

      resBloco->tdeslBTN = 0.0;
      resBloco->ndeslBTN = 0.0;
		if (resBloco->ncons == 0)
			continue; // protecao

      lisResCarga = resCont->LisResCargaBloco[resBloco->bloco];
      for(int nc = 0; nc < lisResCarga->Count; nc++)
         {
         resCarga = (strResCarga*)lisResCarga->Items[nc];
         resCarga->dicBT = 0.0;
         resCarga->ficBT = 0.0;
         resCarga->endBT = 0.0;
         resCarga->dic = resBloco->tdesl/60;
         resCarga->fic = resBloco->ndesl;
         if(!(iMes < 0))
           {
           resCarga->dic_pu[iMes] = resCarga->dic;
           resCarga->fic_pu[iMes] = resCarga->fic;
           }

			if ((paramCarga = paramConf->ParamCarga[resCarga->carga]) == NULL)
				continue;

			if (iMes < 0)
			{
				fatLambda = fatTrep = 1.0;
			}
         else
            {
            fatTrep   = paramCarga->tr_pu[iMes];
            fatLambda = paramCarga->lamb_pu[iMes];
            }
			fatTMA = paramCarga->fatTMA;
			// hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por carga
			trep = fatTMA * fatTrep * paramCarga->trep;
			////hk201703: adaptações para planTurmas: acréscimo de fator de ajuste do tma por carga
         lambda  = fatLambda * paramCarga->lambda;
         //Calculo da contribuição ao dic proveniente de falhas nas redes BT
         resCarga->dicBT = trep * lambda/60;
         resCarga->ficBT = lambda;
         resCarga->dic += resCarga->dicBT;
         resCarga->fic += resCarga->ficBT;
         for(int np1 = 0; np1 < numPat; np1++)
            {
				resCarga->endBT +=
					(distFalhasPat[np1] * resCarga->dicBT * demanda->P_mw[resCarga->carga][np1]);
            }

         if(!(iMes < 0))
           {
           resCarga->dic_pu[iMes] = resCarga->dic;
           resCarga->fic_pu[iMes] = resCarga->fic;
           }

         //Soma dicBT e ficBT em resBloco
         resBloco->tdeslBTN += (resCarga->dicBT * 60) * paramCarga->nconsJus;//resCarga->ncons;
         resBloco->ndeslBTN +=  resCarga->ficBT       * paramCarga->nconsJus;//resCarga->ncons;
         //Soma dicBT e ficBT em resRede
         resRede->tdeslBTN  += (resCarga->dicBT * 60) * paramCarga->nconsJus;//resCarga->ncons;
         resRede->ndeslBTN  +=  resCarga->ficBT       * paramCarga->nconsJus;//resCarga->ncons;
         resRede->endBT     +=  resCarga->endBT;

         //fprintf(fout, "%s\t %s\t %10.4f\t %10.4f\t %i\t %10.4f\t %10.4f\n", rede->Codigo, paramCarga->codCarga, paramCarga->lambda, paramCarga->trep, paramCarga->nconsJus, resRede->tdeslBTN, resRede->ndeslBTN);
         }
      }
   resRede->tdeslN += resRede->tdeslBTN;
   resRede->ndeslN += resRede->ndeslBTN;
   resRede->end    += resRede->endBT;
   //fclose(fout);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::CalculaEstrategiaSoc(VTDefeito *pdef, int iPat)
   {
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTCalcEstratSoc *calcEstratSoc = (VTCalcEstratSoc*)apl->GetObject(__classid(VTCalcEstratSoc));

   //Inicia (estratSoc = pdef->EstratSoc[iPat])->lisBlocosReparo =
   //= pdef->LisBlocosAgrega (que inclui pde->Bloco) + pdef->LisBlocosJusDef
   calcEstratSoc->IniciaEstrategiaSoc(pdef, iPat);

   //Ordena pdef->LisVisRedeDef por capacidade em iPat, buscaProt->FolgaIA(pchave, iPat)
   //   :OrdenaVisRedeCap(pdef, iPat, flagAuto = true);
   //Verifica a melhor opção com chaves automáticas enquanto (estratSoc->lisBlocosSoc->Count < pdef->LisBlocosMan->Count)
   //      :VerificaVis(estratSoc, visRede);
   //           Caso pu_transf < this->pu_lim
   //                estratSoc->AdicionaVisRedeSoc1(visRede, blRadRapida, blRadLenta);
   //                        //Cria manobra de chave Vis
   //                        estratSoc->lisVisRedeSoc->Add(visRedeSoc = NewObjVisRedeSoc(npat));//new TVisRedeSoc(npat));
   //
   //
   //Remove blocos socorridos da lista de blocos desligados durante o reparo
   //      :for(int nbl = 0; nbl < estratSoc->lisBlocosSoc->Count; nbl++)
   //             estratSoc->lisBlocosReparo->Remove(estratSoc->lisBlocosSoc->Items[nbl]);
   //

	if (paramConf->ParamGerais->flagRecurso) //hk201808: reformula
		calcEstratSoc->CalculaEstrategiaSoc(pdef, iPat, paramConf->ParamGerais->pu_lim,
		buscaProt->LisChvAuto, buscaProt->LisChavesAtivas);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::CalculaEstrategiasDefeito(VTDefeito *pdef)
   {
   double *distFalhasPat = paramConf->DistFalhasPat;
   ///////////////////////////////////////////////////////////////
   //Reinicia parametro de profundidade dos religamentos
   ///////////////////////////////////////////////////////////////
   buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;
   ///////////////////////////////////////////////////////////////
   //Gera conjuntos de blocos e define estratégia para tratamento/
   //de blocos a jusante da falha                                /
   ///////////////////////////////////////////////////////////////
	if (!buscaProt->PreparaDefeito(pdef))
		return (false); // hkv201607 caso não haja chave de protecao a montante do defeito

   for(int np = 0; np < numPat; np++)
      {
		if (IsDoubleZero(distFalhasPat[np], 1e-4))
			continue;
      //Inicia (estratSoc = pdef->EstratSoc[iPat])->lisBlocosReparo
      //Ordena pdef->LisVisRedeDef por capacidade em iPat, buscaProt->FolgaIA(pchave, iPat)
      //   :OrdenaVisRedeCap(pdef, iPat, flagAuto = true);
      //Verifica a melhor opção com chaves automáticas enquanto (estratSoc->lisBlocosSoc->Count < pdef->LisBlocosMan->Count)
      //      :VerificaVis(estratSoc, visRede);
      //           Caso pu_transf < this->pu_lim
      //                estratSoc->AdicionaVisRedeSoc1(visRede, blRadRapida, blRadLenta);
      //                        //Cria manobra de chave Vis
      //                        estratSoc->lisVisRedeSoc->Add(visRedeSoc = NewObjVisRedeSoc(npat));//new TVisRedeSoc(npat));
      //
      //
      //Remove blocos socorridos da lista de blocos desligados durante o reparo
      //      :for(int nbl = 0; nbl < estratSoc->lisBlocosSoc->Count; nbl++)
      //             estratSoc->lisBlocosReparo->Remove(estratSoc->lisBlocosSoc->Items[nbl]);
      //

      CalculaEstrategiaSoc(pdef, np);
      }

   //hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
   int nconsProtDef, nconsProtAuto;
   nconsProtDef = NConsLisBloco(pdef->LisBlocosProtDef);
   if(nconsProtDef == 0)
     {
     pdef->ChvIsolaMont = NULL;
     pdef->ChvIsolaAutoMont = NULL;
     }
   else
     {
		if (pdef->ChvIsolaAutoMont)
			// hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
		{
			nconsProtAuto = NConsLisBloco(pdef->LisBlocosProtAuto);
			if (nconsProtAuto == 0)
				pdef->ChvIsolaAutoMont = NULL;
          }
     }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ChaveTelecom(VTChave *pchv) //hk201808: indisp: temp
	{
   bool flagChaveTelecom;
   strDadoEqptoIndisp *eqptoIndisp;

   disponibilidadeChave = 0;
   flagChaveTelecom = ChaveAutomatica(pchv);
   if(flagChaveTelecom == true)
     {
     eqptoIndisp = NULL;
     for(int ne = 0; ne < paramConf->LisEqptoIndisp->Count; ne++)
        {
        eqptoIndisp = (strDadoEqptoIndisp*)paramConf->LisEqptoIndisp->Items[ne];
        if(eqptoIndisp->eqpto == pchv) break;
        eqptoIndisp = NULL;
        }

     if(eqptoIndisp != NULL)
       disponibilidadeChave = eqptoIndisp->vindisp;
     else
       disponibilidadeChave = paramConf->EqptoIndispPadrao->vindisp;
     }
	/*bool flagChaveTelecom = 0;
	strChaveTelecomDisponibilidade *chaveTelecom;
	disponibilidadeChave = 0;
   //protecao para avaliação de manobras ao inicializar
	if (cenario == NULL)
		{
      flagChaveTelecom = ChaveAutomatica(pchv);
		return(flagChaveTelecom);
		}

	for (int i = 0; i < cenario->lisChavesAuto->Count; i++)
		{
		chaveTelecom = (strChaveTelecomDisponibilidade*)cenario->lisChavesAuto->Items[i];
		if (pchv->Codigo.AnsiCompare(chaveTelecom->codigoChaveAuto) == 0)
			{
			disponibilidadeChave = chaveTelecom->disponibilidade;
			flagChaveTelecom = 1;
			break;
			}
		}
   */
   return(flagChaveTelecom);

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::CalculaIndicadoresDefeitoCenario(VTDefeito *pdef) //hk201808: indisp: temp
	{
   VTEstratSoc *estratSoc;
   VTVisRedeSoc *visRedeSoc;
   VTChave     *chaveIsola;
   double      taux, treparoTotal, dcons, tcons, dfalhasPat;
   double      dconsPisca, dconsPiscaVIP, dini, tini;
   int         iPat;
   double      nmanAuto, nmanMan, fatTMA;
   strDadoParamRede *paramRede;
   bool        flagINDIC; //Se o desligamento é computado nos indicadores
	bool        flagAutoVis, flagAutoChvIsolaMont;
	int nconsProtDef;
	// hkrev20160628 acréscimo de critério, sem manobras a montante caso não haja consumidores
	int         nconsProtAuto;
	double disponibilidadeChave_vis = 0;


   ////////////////////////////////////////////////////////////////////////////////
   //--------------------------------------------------------
   //Calcula acrescimo de indicadores devido falta no bloco de falha pdef->bloco
   //--------------------------------------------------------
   //Atualiza resultados:
   //resBloco->ndesl    //tempos e desligamentos (sem multiplicar por ncons) para desligamentos > 3min
   //resBloco->tdesl
   //resBloco->ndeslEND //tempos e desligamentos (sem multiplicar por ncons) totais sem limitantes
   //resBloco->tdeslEND
   //resBloco->end      //end: energia não distribuida

   //201711: acrescentar a taxa de nao coordenacao e o índice da protecao
   //cuidado para não manobrar chave de isolamento fusível

   ////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////
   //Gera conjuntos de blocos e define estratégia para tratamento/
   //de blocos a jusante da falha                                /
   ///////////////////////////////////////////////////////////////

   /*VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTBlocoRad *blocoRad;
   AnsiString arqDefeito = path->DirTmp() + "\\";
   //verificaChave
   bool flagImprime = false;
   blocoRad = buscaProt->IdentificaBlocoRad(pdef->Bloco);
   if(blocoRad->Chave != NULL)
     {if(blocoRad->Chave->Codigo.AnsiCompare("045CF005112409") == 0)
         {
         arqDefeito += "Def_"+ blocoRad->Chave->Codigo + ".txt";
         flagImprime = true;
         }
      if(blocoRad->Chave->Codigo.AnsiCompare("CN:RDUT  0107-045EP005076609_c") == 0)
         {
          arqDefeito += "Def_CN.txt";
          flagImprime = true;
         }
     }
	*/
	if (!CalculaEstrategiasDefeito(pdef))
		return; // hkv201607 caso não haja chave de protecao a montante do defeito

   ///////////////////////////////////////////////////////////////////////////
   //Cálcula o acréscimo aos indicadores de continuidade//////////////////////
   ///////////////////////////////////////////////////////////////////////////
   pdef->NFalhas = pdef->TaxaFalha * pdef->ComprimentoBloco;
   pdef->NPisca  = 0.0;

	if (IsDoubleZero(pdef->NFalhas))
		return;

   //Reduz total de falhas pela instalação de chave religadora
//   if(pdef->ChvProt != NULL)
//    {
//    if((pdef->ChvProt->TipoChave->Tipo == chaveRELIGADORA)||(pdef->ChvProt->TipoChave->Tipo == chaveDISJUNTOR))
//                  pdef->NFalhas = pdef->NFalhas * (1.0 - percDefTrans);
//    }

   if(pdef->ChvReliga != NULL)
	  {
     pdef->NPisca  = pdef->NFalhas * paramConf->ParamGerais->percDefTrans;
     pdef->NFalhas = pdef->NFalhas * (1.0 - paramConf->ParamGerais->percDefTrans);
     //Gera indicador de piscadas
     RecalcIndicPiscadas(pdef);
     }
   //hk201703: adaptações para planTurmas:: acréscimo de fator de ajuste do tma
   fatTMA = pdef->FatTMA;

   dcons    = tcons   = 0.0;
   nmanAuto = nmanMan = 0;

   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   //hk201706: pdef->tlocaliza = paramConf->ParamGerais->tlocaliza;
   if(pdef->FlagTPre == true)
     {
     if(IsDoubleZero(pdef->InfIF->compProt))//pdef->InfIF associado a chave de proteção a montante de pdef
        pdef->FatIFDlo = 1.0;
     else
        pdef->FatIFDlo = pdef->InfIF->compIF / pdef->InfIF->compProt;
     if((pdef->InfIF->chave->TipoChave->Codigo.AnsiCompare("Base Fusível") == 0)&&(pdef->InfIF->flagIF))
         pdef->FatIFPre = 0.3;
     else
         pdef->FatIFPre = 1.0;
     pdef->tlocaliza = pdef->TReparo * (pdef->Ptpre * pdef->FatIFPre  + pdef->Ptdlo * pdef->FatIFDlo);
     //pdef->tlocaliza = pdef->TReparo * (1-pdef->Ptexe);
     pdef->texecucao = pdef->TReparo * pdef->Ptexe;
     }
   else
     {
   pdef->tlocaliza = paramConf->ParamGerais->tlocaliza;
     pdef->texecucao = pdef->TReparo;
     }
   pdef->tman_montante = 0.0;

   //hk201710: referencia tratamento a montante
   treparoTotal = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
   //if(pdef->ChvIsolaMont)
   //  {
   //  //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
   //  flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);
   //  if(flagAutoChvIsolaMont)
   //    {treparoTotal += paramConf->ParamGerais->tman_auto; }//nmanAuto += dfalhasPat;}//nmanAuto++;}
   //  else
   //    {treparoTotal += paramConf->ParamGerais->tman_manual;}// nmanMan += dfalhasPat;}//nmanMan++;}
   //  }
   treparoTotal += pdef->texecucao;  //treparoTotal += pdef->TReparo;//trepCirc;
   treparoTotal *= fatTMA; //hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
   //hk201710: referencia tratamento a montante


//   nconsProtDef = 1;
//   if((pdef->LisBlocosProtDef->Count > 0)&&(nconsProtDef > 0)) //Acrescentar outras restricoes à manobra
	if(pdef->ChvIsolaMont)
	  {//etapa a: isolamento montante do defeito
      dfalhasPat = pdef->NFalhas;

      //Considerar a não coordenação da protecao: melhoria COPEL
      //Acréscimo de desligamento em caso de falha do ajuste com uma taxa fixa, digamos 1%
      //para blocos entre a proteção que deveria atuar e a proteçao imediatamente a montante


		if(pdef->ChvIsolaAutoMont)//hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
		  {
		  //Acao 1: abre chave de isolamento automaática a montante e fecha chave de protecao
        //blocos entre a chave de protecao e a chave automática de isolamento a montante
        taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
        //taux += buscaProt->ChaveAutomatica(pdef->ChvProt) ? tman_auto : tman_manual;

		  //gh201807: alteração para consideração de taxas de disponibilidade de telecom
//		  if(ChaveAutomatica(pdef->ChvProt))
//		  	{
//		  		taux += paramConf->ParamGerais->tman_auto;
//		  		nmanAuto += dfalhasPat;
//		  	} // nmanAuto++;
		  if (ChaveTelecom(pdef->ChvProt))
			{
			taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
			nmanAuto += dfalhasPat;
			}

		  else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;
		  //obter a disponibilidade da chave pdef->ChvIsolaAutoMont para abrir chave automaticada de isolamento
		  ChaveTelecom(pdef->ChvIsolaAutoMont);
		  taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
//		  taux += paramConf->ParamGerais->tman_auto;
        nmanAuto += dfalhasPat;//nmanAuto++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosProtAuto, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
			// hk201703: adaptações para planTurmas: taux * pdef->NFalhas * fatTMA
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManProtAut.tempo = taux;
        pdef->ManProtAut.ndef  = pdef->NFalhas;
        pdef->ManProtAut.nBlocosManobra = pdef->LisBlocosProtAuto->Count;
        pdef->ManProtAut.clih = tcons - tini;
        pdef->ManProtAut.cli  = dcons - dini;
        pdef->ManProtAut.chaveAbre = pdef->ChvIsolaAutoMont;
		  pdef->ManProtAut.chaveFecha= pdef->ChvProt;
        //Acao 2: abre chave a montante ligada ao bloco de defeito e fecha chave automatica a montante do defeito
        //blocos entre a chave de protecao automática e chave a montante do defeito
		  taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
		  //taux += buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;

		  //gh201807: alteração para consideração de taxas de disponibilidade de telecom
//		  if(ChaveAutomatica(pdef->ChvIsolaMont)) //automática sem funcao de protecao
//			{
//				taux += paramConf->ParamGerais->tman_auto;
//				nmanAuto += dfalhasPat;
//			} // nmanAuto++;

		  if (ChaveTelecom(pdef->ChvIsolaMont))
			{
			taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
			nmanAuto += dfalhasPat;
			}
        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

		  //gh201807: alteração para consideração de taxas de disponibilidade de telecom
		  //obter a disponibilidade da chave pdef->ChvIsolaAutoMont para fechar chave automatizada
		  ChaveTelecom(pdef->ChvIsolaAutoMont);
		  //taux += paramConf->ParamGerais->tman_auto;
		  taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
		  nmanAuto += dfalhasPat;//nmanAuto++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosAutoDef, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManAutoDef.tempo = taux;
        pdef->ManAutoDef.ndef  = pdef->NFalhas;
        pdef->ManAutoDef.nBlocosManobra = pdef->LisBlocosAutoDef->Count;
        pdef->ManAutoDef.clih = tcons - tini;
        pdef->ManAutoDef.cli  = dcons - dini;
        pdef->ManAutoDef.chaveAbre = pdef->ChvIsolaMont;
        pdef->ManAutoDef.chaveFecha= pdef->ChvIsolaAutoMont;
		  }
		else
		  {
        //1 etapa de isolamento a montante
        taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;

		  //gh201807: alteração para consideração de taxas de disponibilidade de telecom
//		  if(ChaveAutomatica(pdef->ChvProt))
//			{
//				taux += paramConf->ParamGerais->tman_auto;
//				nmanAuto += dfalhasPat;
//			} // nmanAuto++;
		  if (ChaveTelecom(pdef->ChvProt))
			{
			taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
			nmanAuto += dfalhasPat;
			}

        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

		  //gh201807: alteração para consideração de taxas de disponibilidade de telecom
//        if(ChaveAutomatica(pdef->ChvIsolaMont))
//			{
//				taux += paramConf->ParamGerais->tman_auto;
//				nmanAuto += dfalhasPat;
//			} // nmanAuto++;  //automática sem funcao de protecao

		  if (ChaveTelecom(pdef->ChvIsolaMont))
			{
			taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
			nmanAuto += dfalhasPat;
			}

        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosProtDef, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManProtDef.tempo = taux;
        pdef->ManProtDef.ndef  = pdef->NFalhas;
        pdef->ManProtDef.nBlocosManobra = pdef->LisBlocosProtDef->Count;
        pdef->ManProtDef.clih = tcons - tini;
        pdef->ManProtDef.cli  = dcons - dini;
        pdef->ManProtDef.chaveAbre = pdef->ChvIsolaMont;
        pdef->ManProtDef.chaveFecha= pdef->ChvProt;
		  }
	  }

   //Calculo do tempo dos blocos a justante não transferidos a outros circuitos
   treparoTotal = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
   //hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores

	//hk201710: referencia tratamento a montante
	if(pdef->ChvIsolaMont)
	  {

	  //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
		//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//     flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);
//     if(flagAutoChvIsolaMont)
//		{
//			treparoTotal += paramConf->ParamGerais->tman_auto;
//		} // nmanAuto += dfalhasPat;//nmanAuto++;

	  flagAutoChvIsolaMont = ChaveTelecom(pdef->ChvIsolaMont);
	  if(flagAutoChvIsolaMont)
		{
		treparoTotal += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
		} // nmanAuto += dfalhasPat;//nmanAuto++;

	  else
		{
			treparoTotal += paramConf->ParamGerais->tman_manual;
		} // nmanMan += dfalhasPat;//nmanMan++;
	  }
   //adiciona período de reparo
   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   treparoTotal += pdef->texecucao;  //treparoTotal += pdef->TReparo;//trepCirc;
	treparoTotal *= fatTMA;
	// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
   pdef->TReparoTotal = treparoTotal;
   //flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);

   double *distFalhasPat = paramConf->DistFalhasPat;
   //para cada estratégia de socorro por patamar do dia
	pdef->tman_jusante = 0.0;
   for(int np = 0; np < numPat; np++)
      {
		if (IsDoubleZero(distFalhasPat[np], 1e-4))
			continue;
      //Número de falhas no patamar
      dfalhasPat = distFalhasPat[np] * pdef->NFalhas;

      estratSoc = pdef->EstratSoc[np];
      //etapa b: CÁLCULO DOS INDICADORES DOS BLOCOS DESLIGADOS NO PERÍODO DE REPARO DA FALHA EM np
      //taux = tlocaliza;
      //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
		if (pdef->ChvIsolaMont)
			// hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
		{
			if (flagAutoChvIsolaMont)
          nmanAuto += dfalhasPat;
         else
          nmanMan  += dfalhasPat;//nmanMan++;
        }

      //reparo dos blocos
      flagINDIC = (treparoTotal > paramConf->ParamGerais->tlimIC);
		dini = dcons;
		tini = tcons;
		RecalcIndic(dcons, tcons, estratSoc->lisBlocosReparo, dfalhasPat, treparoTotal * dfalhasPat,
			np, flagINDIC);
      //Guarda valores em pdef
      pdef->ManReparo[np].tempo = treparoTotal;
      pdef->ManReparo[np].ndef  = dfalhasPat;
      pdef->ManReparo[np].nBlocosManobra = estratSoc->lisBlocosReparo->Count;
      pdef->ManReparo[np].clih = tcons - tini;
      pdef->ManReparo[np].cli  = dcons - dini;
      pdef->ManReparo[np].chaveAbre = NULL;
      pdef->ManReparo[np].chaveFecha= NULL;

      //etapa c: indicadores para os blocos a jusante da falha transferidos por vis
      for(int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
			{
			visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
			//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//			flagAutoVis = ChaveAutomatica(visRedeSoc->visRede->pchave);

			flagAutoVis = ChaveTelecom(visRedeSoc->visRede->pchave);
			disponibilidadeChave_vis = disponibilidadeChave;
			if(visRedeSoc->blRadRapida != NULL) //Socorro 2 etapas - rápido -> lento
			  {
           //Socorro etapa rápida
			  chaveIsola = visRedeSoc->blRadRapida->Chave;
			  ChaveTelecom(chaveIsola);
			  taux  = 0.0;//??será? Blocos a jusante podem ser transferidos imediatamente sem tlocaliza;
//           taux += paramConf->ParamGerais->tman_auto; //abre chave de isolamento rápido
			  taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave*disponibilidadeChave_vis + (paramConf->ParamGerais->tman_manual+pdef->tlocaliza) * (1-disponibilidadeChave*disponibilidadeChave_vis));
			  nmanAuto += dfalhasPat;//nmanAuto++;
			  if(flagAutoVis)//fecha chave vis
				{
//					taux += paramConf->ParamGerais->tman_auto;
					taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave*disponibilidadeChave_vis + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave*disponibilidadeChave_vis));
					nmanAuto += dfalhasPat;
				} // nmanAuto++;
           else
				{
					taux += paramConf->ParamGerais->tman_manual;
					nmanMan += dfalhasPat;
				} // nmanMan++;
           //IniNova
				taux *= fatTMA;
				// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
           taux = min(taux, treparoTotal);

           flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
				dini = dcons;
				tini = tcons;
				RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocRapida, dfalhasPat,
					taux * dfalhasPat, np, flagINDIC);
           //Guarda valores em pdef
           pdef->ManSocRapida[np].tempo = taux;
           pdef->ManSocRapida[np].ndef  = dfalhasPat;
           pdef->ManSocRapida[np].nBlocosManobra +=  visRedeSoc->lisBlocosSocRapida->Count;
           pdef->ManSocRapida[np].clih += (tcons - tini);
           pdef->ManSocRapida[np].cli  += (dcons - dini);
           pdef->ManSocRapida[np].chaveAbre = visRedeSoc->blRadRapida->Chave;
           pdef->ManSocRapida[np].chaveFecha= visRedeSoc->visRede->pchave;

           pdef->tman_jusante += (taux * dfalhasPat);

           if(visRedeSoc->blRadLenta != NULL)
					{
               taux  = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;  //??depende de localizacao de falta
					taux += paramConf->ParamGerais->tman_manual;
					// abre chave manual mais proxima do bloco de falha
					nmanMan += dfalhasPat;//nmanMan++;
					//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//					taux += paramConf->ParamGerais->tman_auto;  //fecha chave de manobra rápida
					taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
               nmanAuto += dfalhasPat;//nmanAuto++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               //Guarda valores em pdef
               pdef->ManSocLenta[np].tempo = taux;
               pdef->ManSocLenta[np].ndef  = dfalhasPat;
               pdef->ManSocLenta[np].nBlocosManobra =  visRedeSoc->lisBlocosSocRapida->Count;
               pdef->ManSocLenta[np].clih += (tcons - tini);
               pdef->ManSocLenta[np].cli  += (dcons - dini);
               pdef->ManSocLenta[np].chaveAbre = visRedeSoc->blRadLenta->Chave;
               pdef->ManSocLenta[np].chaveFecha= visRedeSoc->blRadRapida->Chave;

               pdef->tman_jusante += ((taux - pdef->tlocaliza) * dfalhasPat);
					}

			  if(paramConf->ParamGerais->flagRetorno)
					{
               if(flagAutoVis)//abre chave vis
					{
					//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//						taux = paramConf->ParamGerais->tman_auto;
						taux = (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
						nmanAuto += dfalhasPat;
					} // nmanAuto++;
               else
					{
						taux = paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat;
					}
//               taux += paramConf->ParamGerais->tman_auto; //fecha chave de isolamento rápido
					taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
					nmanAuto += dfalhasPat;//nmanAuto++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocRapida, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               }
			  }
         else
			  {
           if(visRedeSoc->blRadLenta != NULL) //socorro em uma etapa lenta
					{
               //Socorro 1 estágio lento
               taux  = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
					taux += paramConf->ParamGerais->tman_manual;
					nmanMan += dfalhasPat; // nmanMan++;
               if(flagAutoVis)//fecha chave vis
					{
					//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//						taux += paramConf->ParamGerais->tman_auto;
						taux += (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
						nmanAuto += dfalhasPat;
					} // nmanAuto++;
               else
					{
						taux += paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat;
					} // nmanMan++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               //Guarda valores em pdef
               pdef->ManSocLenta[np].tempo = taux;
               pdef->ManSocLenta[np].ndef  = dfalhasPat;
               pdef->ManSocLenta[np].nBlocosManobra =  visRedeSoc->lisBlocosSocLenta->Count;
               pdef->ManSocLenta[np].clih += (tcons - tini);
               pdef->ManSocLenta[np].cli  += (dcons - dini);
               pdef->ManSocLenta[np].chaveAbre = visRedeSoc->blRadLenta->Chave;
               pdef->ManSocLenta[np].chaveFecha= visRedeSoc->visRede->pchave;

               pdef->tman_jusante += ((taux - pdef->tlocaliza) * dfalhasPat);

               if(paramConf->ParamGerais->flagRetorno)
					  {
                 //Retorno
                 if(flagAutoVis)//abre chave vis
						{
						//gh201807: alteração para consideração de taxas de disponibilidade de telecom
//							taux = paramConf->ParamGerais->tman_auto;
							taux = (paramConf->ParamGerais->tman_auto*disponibilidadeChave + paramConf->ParamGerais->tman_manual * (1-disponibilidadeChave));
							nmanAuto += dfalhasPat;
						} // nmanAuto++;
					  else
						{
							taux = paramConf->ParamGerais->tman_manual;
							nmanMan += dfalhasPat;
						} // nmanMan++;
						taux += paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat; // nmanMan++;
                 //IniNova
						taux *= fatTMA;
						// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
                 taux = min(taux, treparoTotal);
					  flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
						dini = dcons;
						tini = tcons;
						RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
							taux * dfalhasPat, np, flagINDIC);
                 }
               }
			  }
			}
		}
   //volta a normalizar o tempo de manobra a jusante
   if(!IsDoubleZero(pdef->NFalhas))
       pdef->tman_jusante /= pdef->NFalhas;

   //verificaChave
   //if(flagImprime)
   //   ImprimeManobrasDefeito(pdef, arqDefeito);

   pdef->DCons = dcons; //desligamentos * clientes
	pdef->TCons = tcons; //desligamentos * tempo Desliga * clientes
   pdef->NmanAuto = nmanAuto;//hkedp  * 1000;
   pdef->NmanMan  = nmanMan; //hkedp  * 1000;

   //Calcula deltaDEC, deltaFEC e deltaEND              TDESL                    DESL
//   a -pdef->LisBlocosJusProt;                            //2 tman                   1
//   b -pdef->LisBlocosJusDef + blDef;                     //nsoc * (2tman)           0
//   c -pdef->LisBlocosJusDef + blDef - soma(lisBlocoSoc); //treparo                  0
//   d1-pdef->Bloco                                          //tman                     0
//   d2-pdef->LisBlocosJusDef;                             //2 tman                   1

	}

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::CalculaIndicadoresDefeito(VTDefeito *pdef)
   {
   VTEstratSoc *estratSoc;
   VTVisRedeSoc *visRedeSoc;
   VTChave     *chaveIsola;
   double      taux, treparoTotal, dcons, tcons, dfalhasPat;
   double      dconsPisca, dconsPiscaVIP, dini, tini;
   int         iPat;
   double      nmanAuto, nmanMan, fatTMA;
   strDadoParamRede *paramRede;
   bool        flagINDIC; //Se o desligamento é computado nos indicadores
   bool        flagAutoVis, flagAutoChvIsolaMont;
	int nconsProtDef;
	// hkrev20160628 acréscimo de critério, sem manobras a montante caso não haja consumidores
   int         nconsProtAuto;


   ////////////////////////////////////////////////////////////////////////////////
   //--------------------------------------------------------
   //Calcula acrescimo de indicadores devido falta no bloco de falha pdef->bloco
   //--------------------------------------------------------
   //Atualiza resultados:
   //resBloco->ndesl    //tempos e desligamentos (sem multiplicar por ncons) para desligamentos > 3min
   //resBloco->tdesl
   //resBloco->ndeslEND //tempos e desligamentos (sem multiplicar por ncons) totais sem limitantes
   //resBloco->tdeslEND
   //resBloco->end      //end: energia não distribuida

   //201711: acrescentar a taxa de nao coordenacao e o índice da protecao
   //cuidado para não manobrar chave de isolamento fusível

   ////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////
   //Gera conjuntos de blocos e define estratégia para tratamento/
   //de blocos a jusante da falha                                /
   ///////////////////////////////////////////////////////////////

   /*VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTBlocoRad *blocoRad;
   AnsiString arqDefeito = path->DirTmp() + "\\";
   //verificaChave
   bool flagImprime = false;
   blocoRad = buscaProt->IdentificaBlocoRad(pdef->Bloco);
   if(blocoRad->Chave != NULL)
     {if(blocoRad->Chave->Codigo.AnsiCompare("045CF005112409") == 0)
         {
         arqDefeito += "Def_"+ blocoRad->Chave->Codigo + ".txt";
         flagImprime = true;
         }
      if(blocoRad->Chave->Codigo.AnsiCompare("CN:RDUT  0107-045EP005076609_c") == 0)
         {
          arqDefeito += "Def_CN.txt";
          flagImprime = true;
         }
     }
   */
	if (!CalculaEstrategiasDefeito(pdef))
		return; // hkv201607 caso não haja chave de protecao a montante do defeito

   ///////////////////////////////////////////////////////////////////////////
   //Cálcula o acréscimo aos indicadores de continuidade//////////////////////
   ///////////////////////////////////////////////////////////////////////////
   pdef->NFalhas = pdef->TaxaFalha * pdef->ComprimentoBloco;
   pdef->NPisca  = 0.0;

	if (IsDoubleZero(pdef->NFalhas))
		return;

   //Reduz total de falhas pela instalação de chave religadora
//   if(pdef->ChvProt != NULL)
//    {
//    if((pdef->ChvProt->TipoChave->Tipo == chaveRELIGADORA)||(pdef->ChvProt->TipoChave->Tipo == chaveDISJUNTOR))
//                  pdef->NFalhas = pdef->NFalhas * (1.0 - percDefTrans);
//    }

   if(pdef->ChvReliga != NULL)
     {
     pdef->NPisca  = pdef->NFalhas * paramConf->ParamGerais->percDefTrans;
     pdef->NFalhas = pdef->NFalhas * (1.0 - paramConf->ParamGerais->percDefTrans);
     //Gera indicador de piscadas
     RecalcIndicPiscadas(pdef);
     }
   //hk201703: adaptações para planTurmas:: acréscimo de fator de ajuste do tma
   fatTMA = pdef->FatTMA;

   dcons    = tcons   = 0.0;
   nmanAuto = nmanMan = 0;

   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   //hk201706: pdef->tlocaliza = paramConf->ParamGerais->tlocaliza;
   if(pdef->FlagTPre == true)
     {
     if(IsDoubleZero(pdef->InfIF->compProt))//pdef->InfIF associado a chave de proteção a montante de pdef
        pdef->FatIFDlo = 1.0;
     else
		  pdef->FatIFDlo = pdef->InfIF->compIF / pdef->InfIF->compProt;
	  if((pdef->InfIF->chave->TipoChave->Codigo.AnsiCompare("Base Fusível") == 0)&&(pdef->InfIF->flagIF))
			//gh201806 - alteração para permitir ao usuário alterar o percentual de redução do tempo de preparo, devido à instalação de IF em fusível
			pdef->FatIFPre = (1 - paramConf->ParamGerais->red_tprep_IF);
	  else
			pdef->FatIFPre = 1.0;
     pdef->tlocaliza = pdef->TReparo * (pdef->Ptpre * pdef->FatIFPre  + pdef->Ptdlo * pdef->FatIFDlo);
     //pdef->tlocaliza = pdef->TReparo * (1-pdef->Ptexe);
     pdef->texecucao = pdef->TReparo * pdef->Ptexe;
     }
   else
     {
   pdef->tlocaliza = paramConf->ParamGerais->tlocaliza;
     pdef->texecucao = pdef->TReparo;
     }
   pdef->tman_montante = 0.0;

   //hk201710: referencia tratamento a montante
   treparoTotal = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
   //if(pdef->ChvIsolaMont)
   //  {
   //  //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
   //  flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);
   //  if(flagAutoChvIsolaMont)
   //    {treparoTotal += paramConf->ParamGerais->tman_auto; }//nmanAuto += dfalhasPat;}//nmanAuto++;}
   //  else
   //    {treparoTotal += paramConf->ParamGerais->tman_manual;}// nmanMan += dfalhasPat;}//nmanMan++;}
   //  }
   treparoTotal += pdef->texecucao;  //treparoTotal += pdef->TReparo;//trepCirc;
   treparoTotal *= fatTMA; //hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
   //hk201710: referencia tratamento a montante


//   nconsProtDef = 1;
//   if((pdef->LisBlocosProtDef->Count > 0)&&(nconsProtDef > 0)) //Acrescentar outras restricoes à manobra
   if(pdef->ChvIsolaMont)
     {//etapa a: isolamento montante do defeito
      dfalhasPat = pdef->NFalhas;

      //Considerar a não coordenação da protecao: melhoria COPEL
      //Acréscimo de desligamento em caso de falha do ajuste com uma taxa fixa, digamos 1%
      //para blocos entre a proteção que deveria atuar e a proteçao imediatamente a montante


      if(pdef->ChvIsolaAutoMont)//hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
        {
        //Acao 1: abre chave de isolamento automaática a montante e fecha chave de protecao
        //blocos entre a chave de protecao e a chave automática de isolamento a montante
        taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
        //taux += buscaProt->ChaveAutomatica(pdef->ChvProt) ? tman_auto : tman_manual;
        if(ChaveAutomatica(pdef->ChvProt)) //Cosim: if (ChaveTelecom(pdef->ChvProt))
			{
				taux += paramConf->ParamGerais->tman_auto;
				nmanAuto += dfalhasPat;
			} // nmanAuto++;
        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

        taux += paramConf->ParamGerais->tman_auto; //Cosim: (ChaveTelecom(pdef->ChvIsolaAutoMont))
        nmanAuto += dfalhasPat;//nmanAuto++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosProtAuto, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
			// hk201703: adaptações para planTurmas: taux * pdef->NFalhas * fatTMA
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManProtAut.tempo = taux;
        pdef->ManProtAut.ndef  = pdef->NFalhas;
        pdef->ManProtAut.nBlocosManobra = pdef->LisBlocosProtAuto->Count;
        pdef->ManProtAut.clih = tcons - tini;
        pdef->ManProtAut.cli  = dcons - dini;
        pdef->ManProtAut.chaveAbre = pdef->ChvIsolaAutoMont;
        pdef->ManProtAut.chaveFecha= pdef->ChvProt;
        //Acao 2: abre chave a montante ligada ao bloco de defeito e fecha chave automatica a montante do defeito
        //blocos entre a chave de protecao automática e chave a montante do defeito
        taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
        //taux += buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
        if(ChaveAutomatica(pdef->ChvIsolaMont)) //automática sem funcao de protecao
			{  //Cosim: if (ChaveTelecom(pdef->ChvIsolaMont))
				taux += paramConf->ParamGerais->tman_auto;
				nmanAuto += dfalhasPat;
			} // nmanAuto++;
        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

        taux += paramConf->ParamGerais->tman_auto; //Cosim: ChaveTelecom(pdef->ChvIsolaAutoMont);
        nmanAuto += dfalhasPat;//nmanAuto++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosAutoDef, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManAutoDef.tempo = taux;
        pdef->ManAutoDef.ndef  = pdef->NFalhas;
        pdef->ManAutoDef.nBlocosManobra = pdef->LisBlocosAutoDef->Count;
        pdef->ManAutoDef.clih = tcons - tini;
        pdef->ManAutoDef.cli  = dcons - dini;
        pdef->ManAutoDef.chaveAbre = pdef->ChvIsolaMont;
        pdef->ManAutoDef.chaveFecha= pdef->ChvIsolaAutoMont;
        } //if(pdef->ChvIsolaAutoMont)
      else
        {
        //1 etapa de isolamento a montante
        taux = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
        if(ChaveAutomatica(pdef->ChvProt))
			{
				taux += paramConf->ParamGerais->tman_auto;
				nmanAuto += dfalhasPat;
			} // nmanAuto++;
        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

        if(ChaveAutomatica(pdef->ChvIsolaMont))
			{
				taux += paramConf->ParamGerais->tman_auto;
				nmanAuto += dfalhasPat;
			} // nmanAuto++;  //automática sem funcao de protecao
        else
			{
				taux += paramConf->ParamGerais->tman_manual;
				nmanMan += dfalhasPat;
			} // nmanMan++;

			taux *= fatTMA;
			// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
        taux = min(taux, treparoTotal);//hk201710: referencia tratamento a montante
        flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
			dini = dcons;
			tini = tcons;
			RecalcIndic(dcons, tcons, pdef->LisBlocosProtDef, pdef->NFalhas, taux * pdef->NFalhas,
				iPat = -1, flagINDIC);
        //Guarda valores em pdef
        pdef->tman_montante += (taux - pdef->tlocaliza);
        pdef->ManProtDef.tempo = taux;
        pdef->ManProtDef.ndef  = pdef->NFalhas;
        pdef->ManProtDef.nBlocosManobra = pdef->LisBlocosProtDef->Count;
        pdef->ManProtDef.clih = tcons - tini;
        pdef->ManProtDef.cli  = dcons - dini;
        pdef->ManProtDef.chaveAbre = pdef->ChvIsolaMont;
        pdef->ManProtDef.chaveFecha= pdef->ChvProt;
        }  //if(pdef->ChvIsolaAutoMont)
     }  //if(pdef->ChvIsolaMont)

   //Calculo do tempo dos blocos a justante não transferidos a outros circuitos
   treparoTotal = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
   //hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores

   //hk201710: referencia tratamento a montante
   if(pdef->ChvIsolaMont)
     {
     //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
     flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);
     if(flagAutoChvIsolaMont)
		{
			treparoTotal += paramConf->ParamGerais->tman_auto;
		} // nmanAuto += dfalhasPat;//nmanAuto++;
     else
		{
			treparoTotal += paramConf->ParamGerais->tman_manual;
		} // nmanMan += dfalhasPat;//nmanMan++;
     }
   //adiciona período de reparo
   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   treparoTotal += pdef->texecucao;  //treparoTotal += pdef->TReparo;//trepCirc;
	treparoTotal *= fatTMA;
	// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
   pdef->TReparoTotal = treparoTotal;
   //flagAutoChvIsolaMont = ChaveAutomatica(pdef->ChvIsolaMont);

   double *distFalhasPat = paramConf->DistFalhasPat;
   //para cada estratégia de socorro por patamar do dia
   pdef->tman_jusante = 0.0;
   for(int np = 0; np < numPat; np++)
      {
		if (IsDoubleZero(distFalhasPat[np], 1e-4))
			continue;
      //Número de falhas no patamar
      dfalhasPat = distFalhasPat[np] * pdef->NFalhas;

      estratSoc = pdef->EstratSoc[np];
      //etapa b: CÁLCULO DOS INDICADORES DOS BLOCOS DESLIGADOS NO PERÍODO DE REPARO DA FALHA EM np
      //taux = tlocaliza;
      //taux+= buscaProt->ChaveAutomatica(pdef->ChvIsolaMont) ? tman_auto : tman_manual;
		if (pdef->ChvIsolaMont)
			// hk201702: teste para execução de isolamentos no cálculo de indicadores: Faz consistencia com número de consumidores
		{
			if (flagAutoChvIsolaMont)
          nmanAuto += dfalhasPat;
         else
          nmanMan  += dfalhasPat;//nmanMan++;
        }

      //reparo dos blocos
      flagINDIC = (treparoTotal > paramConf->ParamGerais->tlimIC);
		dini = dcons;
		tini = tcons;
		RecalcIndic(dcons, tcons, estratSoc->lisBlocosReparo, dfalhasPat, treparoTotal * dfalhasPat,
			np, flagINDIC);
      //Guarda valores em pdef
      pdef->ManReparo[np].tempo = treparoTotal;
      pdef->ManReparo[np].ndef  = dfalhasPat;
      pdef->ManReparo[np].nBlocosManobra = estratSoc->lisBlocosReparo->Count;
      pdef->ManReparo[np].clih = tcons - tini;
      pdef->ManReparo[np].cli  = dcons - dini;
      pdef->ManReparo[np].chaveAbre = NULL;
      pdef->ManReparo[np].chaveFecha= NULL;

      //etapa c: indicadores para os blocos a jusante da falha transferidos por vis
      for(int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
         {
         visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
         flagAutoVis = ChaveAutomatica(visRedeSoc->visRede->pchave);
         if(visRedeSoc->blRadRapida != NULL) //Socorro 2 etapas - rápido -> lento

           {
           //Socorro etapa rápida
           chaveIsola = visRedeSoc->blRadRapida->Chave;
           taux  = 0.0;//??será? Blocos a jusante podem ser transferidos imediatamente sem tlocaliza;
           taux += paramConf->ParamGerais->tman_auto; //abre chave de isolamento rápido
           nmanAuto += dfalhasPat;//nmanAuto++;
           if(flagAutoVis)//fecha chave vis
				{
					taux += paramConf->ParamGerais->tman_auto;
					nmanAuto += dfalhasPat;
				} // nmanAuto++;
           else
				{
					taux += paramConf->ParamGerais->tman_manual;
					nmanMan += dfalhasPat;
				} // nmanMan++;
           //IniNova
				taux *= fatTMA;
				// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
           taux = min(taux, treparoTotal);

           flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
				dini = dcons;
				tini = tcons;
				RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocRapida, dfalhasPat,
					taux * dfalhasPat, np, flagINDIC);
           //Guarda valores em pdef
           pdef->ManSocRapida[np].tempo = taux;
           pdef->ManSocRapida[np].ndef  = dfalhasPat;
           pdef->ManSocRapida[np].nBlocosManobra +=  visRedeSoc->lisBlocosSocRapida->Count;
           pdef->ManSocRapida[np].clih += (tcons - tini);
           pdef->ManSocRapida[np].cli  += (dcons - dini);
           pdef->ManSocRapida[np].chaveAbre = visRedeSoc->blRadRapida->Chave;
           pdef->ManSocRapida[np].chaveFecha= visRedeSoc->visRede->pchave;

           pdef->tman_jusante += (taux * dfalhasPat);

           if(visRedeSoc->blRadLenta != NULL)
               {
               taux  = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;  //??depende de localizacao de falta
					taux += paramConf->ParamGerais->tman_manual;
					// abre chave manual mais proxima do bloco de falha
               nmanMan += dfalhasPat;//nmanMan++;
               taux += paramConf->ParamGerais->tman_auto;  //fecha chave de manobra rápida
               nmanAuto += dfalhasPat;//nmanAuto++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               //Guarda valores em pdef
               pdef->ManSocLenta[np].tempo = taux;
               pdef->ManSocLenta[np].ndef  = dfalhasPat;
               pdef->ManSocLenta[np].nBlocosManobra =  visRedeSoc->lisBlocosSocRapida->Count;
               pdef->ManSocLenta[np].clih += (tcons - tini);
               pdef->ManSocLenta[np].cli  += (dcons - dini);
               pdef->ManSocLenta[np].chaveAbre = visRedeSoc->blRadLenta->Chave;
               pdef->ManSocLenta[np].chaveFecha= visRedeSoc->blRadRapida->Chave;

               pdef->tman_jusante += ((taux - pdef->tlocaliza) * dfalhasPat);
               }

           if(paramConf->ParamGerais->flagRetorno)
               {
               if(flagAutoVis)//abre chave vis
					{
						taux = paramConf->ParamGerais->tman_auto;
						nmanAuto += dfalhasPat;
					} // nmanAuto++;
               else
					{
						taux = paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat;
					}
               taux += paramConf->ParamGerais->tman_auto; //fecha chave de isolamento rápido
               nmanAuto += dfalhasPat;//nmanAuto++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocRapida, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               }
           }
         else
           {
           if(visRedeSoc->blRadLenta != NULL) //socorro em uma etapa lenta
               {
               //Socorro 1 estágio lento
               taux  = pdef->tlocaliza; //hk201706:ModIF  paramConf->ParamGerais->tlocaliza;
					taux += paramConf->ParamGerais->tman_manual;
					nmanMan += dfalhasPat; // nmanMan++;
               if(flagAutoVis)//fecha chave vis
					{
						taux += paramConf->ParamGerais->tman_auto;
						nmanAuto += dfalhasPat;
					} // nmanAuto++;
               else
					{
						taux += paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat;
					} // nmanMan++;
               //IniNova
					taux *= fatTMA;
					// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
               taux = min(taux, treparoTotal);
               flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
					dini = dcons;
					tini = tcons;
					RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
						taux * dfalhasPat, np, flagINDIC);
               //Guarda valores em pdef
               pdef->ManSocLenta[np].tempo = taux;
               pdef->ManSocLenta[np].ndef  = dfalhasPat;
               pdef->ManSocLenta[np].nBlocosManobra =  visRedeSoc->lisBlocosSocLenta->Count;
               pdef->ManSocLenta[np].clih += (tcons - tini);
               pdef->ManSocLenta[np].cli  += (dcons - dini);
               pdef->ManSocLenta[np].chaveAbre = visRedeSoc->blRadLenta->Chave;
               pdef->ManSocLenta[np].chaveFecha= visRedeSoc->visRede->pchave;

               pdef->tman_jusante += ((taux - pdef->tlocaliza) * dfalhasPat);

               if(paramConf->ParamGerais->flagRetorno)
                 {
                 //Retorno
                 if(flagAutoVis)//abre chave vis
						{
							taux = paramConf->ParamGerais->tman_auto;
							nmanAuto += dfalhasPat;
						} // nmanAuto++;
                 else
						{
							taux = paramConf->ParamGerais->tman_manual;
							nmanMan += dfalhasPat;
						} // nmanMan++;
						taux += paramConf->ParamGerais->tman_manual;
						nmanMan += dfalhasPat; // nmanMan++;
                 //IniNova
						taux *= fatTMA;
						// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
                 taux = min(taux, treparoTotal);
                 flagINDIC = (taux > paramConf->ParamGerais->tlimIC);
						dini = dcons;
						tini = tcons;
						RecalcIndic(dcons, tcons, visRedeSoc->lisBlocosSocLenta, dfalhasPat,
							taux * dfalhasPat, np, flagINDIC);
                 }
               }
           }
         }
      }
   //volta a normalizar o tempo de manobra a jusante
   if(!IsDoubleZero(pdef->NFalhas))
       pdef->tman_jusante /= pdef->NFalhas;

   //verificaChave
   //if(flagImprime)
   //   ImprimeManobrasDefeito(pdef, arqDefeito);

   pdef->DCons = dcons; //desligamentos * clientes
   pdef->TCons = tcons; //desligamentos * tempo Desliga * clientes
   pdef->NmanAuto = nmanAuto;//hkedp  * 1000;
   pdef->NmanMan  = nmanMan; //hkedp  * 1000;

   //Calcula deltaDEC, deltaFEC e deltaEND              TDESL                    DESL
//   a -pdef->LisBlocosJusProt;                            //2 tman                   1
//   b -pdef->LisBlocosJusDef + blDef;                     //nsoc * (2tman)           0
//   c -pdef->LisBlocosJusDef + blDef - soma(lisBlocoSoc); //treparo                  0
//   d1-pdef->Bloco                                          //tman                     0
//   d2-pdef->LisBlocosJusDef;                             //2 tman                   1

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RecalcIndicPiscadas(VTDefeito *pdef)
   {
   VTBloco *blCalc;
   TList   *lisBlocoReliga;

   //flagINDIC = true : indicadores são incrementados
   //Retorna os blocos de religamento a jusante da chave religadora
   //if((lisBlocoReliga = buscaProt->ListaZonaReliga(pdef->ChvReliga)) == NULL) return;
	if ((lisBlocoReliga = buscaProt->LisBlJusChave(pdef->ChvReliga)) == NULL)
		return;
   for(int n = 0; n < lisBlocoReliga->Count; n++)
      {
      blCalc = (VTBloco*)lisBlocoReliga->Items[n];
      RecalcIndicPiscadasBloco(blCalc, pdef->NPisca);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RecalcIndicPiscadasBloco(VTBloco *blCalc, double ddesl)
   {
   strResBloco *resBloco;
   if(blCalc->Obj == NULL) return;
   resBloco = (strResBloco*)(blCalc->Obj);//hk201902: temp:
   //hk201902: temp: resBloco = resCont->ResBloco[blCalc];
   //hk201902: temp: 	if (resBloco == NULL)
   //hk201902: temp: 		return;


   resBloco->npiscadas += ddesl;
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RecalcIndic(double &dcons, double &tcons, TList *lisBlCalc,
	double ddesl, double dtempo, int np, bool flagINDIC)
   {
   VTBloco *blCalc;
   //flagINDIC = true : indicadores são incrementados
   for(int n = 0; n < lisBlCalc->Count; n++)
      {
      blCalc = (VTBloco*)lisBlCalc->Items[n];
      RecalcIndicBloco(dcons, tcons, blCalc, ddesl, dtempo, np, flagINDIC);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCalcIndCont::NConsLisBloco(TList *lisBlocos)
   {
   int ncons;
   VTBloco *bloco;
   strResBloco *resBloco;
   ncons = 0;
   for(int n = 0; n < lisBlocos->Count; n++)
      {
	  bloco = (VTBloco*)lisBlocos->Items[n];
	  if(bloco->Obj == NULL) continue;
	  resBloco = (strResBloco*)(bloco->Obj);//hk201902: temp:
	  //hk201902: resBloco = resCont->ResBloco[bloco];
	  //hk201902: 	if (resBloco == NULL)
	  //hk201902: 		continue;
      ncons += resBloco->ncons;
	  }
   return(ncons);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RecalcIndicBloco(double &dcons, double &tcons, VTBloco *blCalc,
	double ddesl, double dtempo, int np, bool flagINDIC)
   {
   strResBloco *resBloco;
   double      *distFalhasPat = paramConf->DistFalhasPat;
   //Atualiza resultados:
   //resBloco->ndesl    //tempos e desligamentos para desligamentos > 3min
   //resBloco->tdesl
   //resBloco->ndeslEND //tempos e desligamentos totais sem limitantes
   //resBloco->tdeslEND
   //resBloco->end      //end e energia não distribuida

   //flagINDIC = true : indicadores são incrementados (limite minimo 3min)
   if(blCalc->Obj == NULL) return;
   resBloco = (strResBloco*)(blCalc->Obj);//hk201902: temp:
   //hk201902: temp: resBloco = resCont->ResBloco[blCalc];
   //hk201902: temp: 	if (resBloco == NULL)
   //hk201902: temp: 		return;

   if(flagINDIC)
	{
		resBloco->ndesl += ddesl; // desligamento
	  resBloco->tdesl    += dtempo;//desligamentos * tempoDesliga
	  dcons += (ddesl  * resBloco->ncons);
	  tcons += (dtempo * resBloco->ncons);
	 }
   resBloco->ndeslEND += ddesl;
   resBloco->tdeslEND += dtempo;
   if(np < 0)
	 {
	 for(int np1 = 0; np1 < numPat; np1++)
		{
			resBloco->end +=
				(distFalhasPat[np1] * dtempo * resBloco->blocoRad->CargaBloco[np1] / 60.);
			// resBloco->cBloco->cargaMW[np1]);
		}
	 }
   else
	 {
     resBloco->end      += dtempo * resBloco->blocoRad->CargaBloco[np] / 60.;//cBloco->cargaMW[np];
     }
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ImprimeImpactos(TLisDefeitoRede *pDefeitoRede)
   {
   VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));
   FILE        *fout1, *fout2;
   VTDefeito   *pdef;
   strResBloco *resBloco;
   AnsiString   arq;

   arq = path->DirTmp() + "\\impacDefeito.csv";
   fout1 = fopen(arq.c_str(), "wt");
   arq = path->DirTmp() + "\\impacRes.csv";
   fout2 = fopen(arq.c_str(), "wt");
   for(int ndef = 0; ndef < pDefeitoRede->lisDefeitos->Count; ndef++)
      {
	  pdef = (VTDefeito*)pDefeitoRede->lisDefeitos->Items[ndef];
	  if(pdef->Bloco->Obj == NULL) continue;
	  resBloco = (strResBloco*)(pdef->Bloco->Obj);//hk201902: temp:
	  //hk201902: temp:resBloco = resCont->ResBloco[pdef->Bloco];
	  //hk201902: temp:	if (resBloco == NULL)
	  //hk201902: temp:		continue;
		if (resBloco->ncons == 0)
			continue;

		fprintf(fout1, "%i; %s; %6.2f; %6.2f; %6.2f\n", ndef, pdef->ChvProt->Codigo.c_str(), pdef->NFalhas,
			pdef->DCons, pdef->TCons);
		fprintf(fout2, "%i; %i; %6.2f; %6.2f\n", ndef, resBloco->ncons, resBloco->ndesl,
			resBloco->tdesl);
      }
   fclose(fout1);
   fclose(fout2);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::VerificaVis(VTEstratSoc *estratSoc, VTVisRede *visRede)
   {
   TList  *lisBlRadJus, *lisAux;
   double icirc, folga, cargaMw, pu_transf;
   VTBlocoRad *blRad, *blRadRapida, *blRadLenta;
   int index;

   icirc = visRede->IA_Crit(estratSoc->npat);
   folga = visRede->FolgaIA(estratSoc->npat);
//n14   icirc = buscaProt->IA_Crit(visRede, estratSoc->npat);
//n14   folga = buscaProt->FolgaIA(visRede, estratSoc->npat);

   //Inicia blocosRad
   blRadRapida = NULL;
   blRadLenta  = NULL;

   //Sequencia rápida
   blRad = NULL;
   index = -1;
   //Verifica as possibilidades de transferencia a com isolamento partir da chave
   //da chave imediatamente a jusante do bloco de defeito
   for(int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= 0; n--)
        {
        //BlocoRad a jusante do bloco de defeito
        blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];
        //Verifica se a chave de isolamento é automática
		if (!ChaveAutomatica(blRad->Chave))
		{
			blRad = NULL;
			continue;
		}

        //Verifica se blRad já foi socorrido - impede formação de malha - em estratSoc->lisBlocosSoc
		if (estratSoc->BlocoSoc(blRad->Bloco))
		{
			blRad = NULL;
			continue;
		}
        for(int nb = 0; nb < blRad->LisBlocoJus->Count; nb++)
		{
			if (estratSoc->BlocoSoc((VTBloco*)blRad->LisBlocoJus->Items[nb]))
			{
				blRad = NULL;
              break;
             }
           }
		if (blRad == NULL)
			continue;

        cargaMw = blRad->CargaBlRad[estratSoc->npat];
        if(icirc > 0.0)
			pu_transf = (icirc - folga + 1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205)
				) / icirc; // (pchave->pbarra1->vnom * 1.73205)) / icirc;
        else
          pu_transf = 99.0;

		if (pu_transf <= paramConf->ParamGerais->pu_lim)
		{
			index = n + 1;
			break;
		}
        blRad = NULL;
        }
   //blRad é NULL ->não ha chaves automáticas, nenhuma automática é viável
   blRadRapida = blRad;

	if ((blRadRapida == NULL) && (index < 0))
		index = 0;

   if((blRadRapida == NULL)||(index >= 0))
     {
     blRad = NULL;
     //busca operacao lenta
     for(int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= index; n--)
        {
        //BlocoRad a jusante do bloco de defeito
        blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];

        //Verifica se blRad já foi socorrido - impede formação de malha - em estratSoc->lisBlocosSoc
			if (estratSoc->BlocoSoc(blRad->Bloco))
			{
				blRad = NULL;
				continue;
			}
        for(int nb = 0; nb < blRad->LisBlocoJus->Count; nb++)
           {
           if(blRadRapida != NULL)
             {//Verifica se bloco esta contido em manobra rápida
					if (blRadRapida->LisBlocoJus->IndexOf(blRad->LisBlocoJus->Items[nb]) != -1)
						continue;
             }
           if(estratSoc->BlocoSoc((VTBloco*)blRad->LisBlocoJus->Items[nb]))
				{
					blRad = NULL;
					break;
           }
			}
			if (blRad == NULL)
				continue;

        cargaMw = blRad->CargaBlRad[estratSoc->npat];
        if(icirc > 0.0)
				pu_transf =
					(icirc - folga + 1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205))
					/ icirc; // (pchave->pbarra1->vnom * 1.73205)) / icirc;
        else
          pu_transf = 99.0;

			if (pu_transf <= paramConf->ParamGerais->pu_lim)
			{
				index = n;
				break;
			}
        blRad = NULL;
        }
     blRadLenta = blRad;
     }
   //Adiciona estratégia de socorro
   estratSoc->AdicionaVisRedeSoc1(visRede, blRadRapida, blRadLenta);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
bool __fastcall TCalcIndCont::VarZonaProt(VTChave *pchave,    int    &nconsProt,
                                          double  &cargaProt, double &distProt)
{
    VTBloco     *bloco;
    TList       *lisBlocoExt;
    strResBloco *resBloco;
    /////////////////////////////////////////////////////////////
    //Funcao que gera variáveis da zona de protecao de pchave ///
    /////////////////////////////////////////////////////////////
    lisBlocoExt = new TList();
    if (!buscaProt->ListaZonaProt(pchave, lisBlocoExt))
    {
        delete lisBlocoExt;
        return (false);
    }
    distProt   = 0.0;
    cargaProt  = 0.0;
    nconsProt  = 0;
    for(int nb = 0; nb < lisBlocoExt->Count; nb++)
    {
        bloco     = (VTBloco*)lisBlocoExt->Items[nb];
        distProt += ComprimentoBloco(bloco);
        if(bloco->Obj == NULL)
            continue;
        resBloco  = (strResBloco*)(bloco->Obj);//hk201902: temp:
        //hk201902: temp:resBloco = resCont->ResBloco[bloco];
        //hk201902: temp:	if (resBloco == NULL)
        //hk201902: temp:		continue;

        for (int np = 0; np < numPat; np++)
            cargaProt += resBloco->blocoRad->CargaBloco[np]; // resBloco->cBloco->cargaMW[np];
        nconsProt += resBloco->ncons;
    }

   delete lisBlocoExt;
   return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ChaveAutomatica(VTChave *pchv)
   {
   return(buscaProt->LisChvAuto->IndexOf(pchv) != -1);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ReiniciaChavesNovas(void)
   {
   lisChavesNovas->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereChaveNova(VTChave *pchave)
   {
   //Muda status de lisChavesNovas para true : ainda não construída
   pchave->Bloqueada = true;

   //Remove chave na lista de chaves ativas hkedp
   if(lisChavesNovas->IndexOf(pchave) == -1)
                         lisChavesNovas->Add(pchave);
   //Remove chave na lista de chaves ativas hkedp hk201808: reformula lisChavesAtivas
   if(buscaProt->LisChavesAtivas->IndexOf(pchave) != -1)
                         buscaProt->LisChavesAtivas->Remove(pchave);
   //se é automátiva insere em lisChavesAtivas
   if(buscaProt->LisChvAuto->IndexOf(pchave) != -1)
                         buscaProt->LisChavesAtivas->Add(pchave);

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::RemoveChaveNova(VTChave *pchave)
   {
	if (lisChavesNovas->IndexOf(pchave) == -1)
		return;
	while (lisChavesNovas->IndexOf(pchave) != -1)
		lisChavesNovas->Remove(pchave);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ImprimeManobrasCjtoDefeitos(TLisDefeitoRede *pDefeitoRede,
	AnsiString arqSaida, bool flagAppend)
   {
   FILE      *fout;
   VTBlocoRad *blocoRad;
   VTDefeito *pdef;
   AnsiString aux, codRede;

   if(flagAppend)
      fout = fopen(arqSaida.c_str(), "a");
   else
      fout = fopen(arqSaida.c_str(), "wt");

   if(pDefeitoRede == NULL)
             {
             fprintf(fout, "ndef\t codRede\t ChvProt");
             fprintf(fout, "\t NFalhas");
             fprintf(fout, "\t TLocaliza");
             fprintf(fout, "\t TManMontante");
             fprintf(fout, "\t TManJusante");
             fprintf(fout, "\t TReparo");
             fprintf(fout, "\t TReparoTotal");
             fprintf(fout, "\t TCons");
             fprintf(fout, "\t ManProtAut.clih");
             fprintf(fout, "\t ManAutoDef.clih");
             fprintf(fout, "\t ManProtDef.clih");
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManReparo[%i].clih   ", np);
                }
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManSocRapida[%i].clih", np);
                }
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManSocLenta[%i].clih ", np);
                }
             fprintf(fout, "\t DCons");
             fprintf(fout, "\t ManProtAut.cli");
             fprintf(fout, "\t ManAutoDef.cli");
             fprintf(fout, "\t ManProtDef.cli");
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManReparo[%i].cli   ", np);
                }
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManSocRapida[%i].cli", np);
                }
             for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t ManSocLenta[%i].cli ", np);
                }
             fprintf(fout, "\n");
             fclose(fout);
             return;
             }

   codRede = pDefeitoRede->rede->Codigo;
   for(int ndef = 0; ndef < pDefeitoRede->lisDefeitos->Count; ndef++)
      {
      pdef = (VTDefeito*)pDefeitoRede->lisDefeitos->Items[ndef];
		if (pdef->ChvProt == NULL)
			aux = "-";
		else
			aux = pdef->ChvProt->Codigo;

	  fprintf(fout, "%i\t %s\t %s", ndef, codRede.c_str(), aux.c_str());
      fprintf(fout, "\t %6.2f", pdef->NFalhas);
      fprintf(fout, "\t %6.2f", pdef->tlocaliza);
      fprintf(fout, "\t %6.2f", pdef->tman_montante);
      fprintf(fout, "\t %6.2f", pdef->tman_jusante);
      fprintf(fout, "\t %6.2f", pdef->TReparo);
      fprintf(fout, "\t %6.2f", pdef->TReparoTotal);

      fprintf(fout, "\t %6.2f", pdef->TCons);
      fprintf(fout, "\t %6.2f", pdef->ManProtAut.clih);
      fprintf(fout, "\t %6.2f", pdef->ManAutoDef.clih);
      fprintf(fout, "\t %6.2f", pdef->ManProtDef.clih);
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManReparo[np].clih);
         }
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManSocRapida[np].clih);
         }
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManSocLenta[np].clih);
         }

      fprintf(fout, "\t %6.2f", pdef->DCons);
      fprintf(fout, "\t %6.2f", pdef->ManProtAut.cli);
      fprintf(fout, "\t %6.2f", pdef->ManAutoDef.cli);
      fprintf(fout, "\t %6.2f", pdef->ManProtDef.cli);
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManReparo[np].cli);
         }
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManSocRapida[np].cli);
         }
      for(int np = 0; np < 24; np++)
		{
			fprintf(fout, "\t %6.2f", pdef->ManSocLenta[np].cli);
         }

      fprintf(fout, "\n");
      }

   fclose(fout);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ImprimeManobrasDefeito(VTDefeito *pdef, AnsiString arqSaida)
   {
   FILE      *fout;
   VTBlocoRad *blocoRad;
   AnsiString aux;
   //VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   //AnsiString arq, aux;
   //arq = path->DirTmp() + "\\" + arqSaida;
   fout = fopen(arqSaida.c_str(), "wt");

   blocoRad = buscaProt->IdentificaBlocoRad(pdef->Bloco);
   aux = "-";
   if(blocoRad != NULL)
	{
		if (blocoRad->Chave != NULL)
			aux = blocoRad->Chave->Codigo;
     }
   fprintf(fout, "Defeito\n", aux);
   fprintf(fout, "%s\n", aux.c_str());

   fprintf(fout, "ManProtAut\n");
   fprintf(fout, "%6.2f;", pdef->ManProtAut.tempo);
   fprintf(fout, "%10.6f;", pdef->ManProtAut.ndef);
   fprintf(fout, "%i;", pdef->ManProtAut.nBlocosManobra);
   fprintf(fout, "%6.2f;", pdef->ManProtAut.clih);
   fprintf(fout, "%6.2f;", pdef->ManProtAut.cli);
	aux = "-";
	if (pdef->ManProtAut.chaveAbre != NULL)
		aux = pdef->ManProtAut.chaveAbre->Codigo;
   fprintf(fout, "%s;", aux.c_str());
	aux = "-";
	if (pdef->ManProtAut.chaveFecha != NULL)
		aux = pdef->ManProtAut.chaveFecha->Codigo;
   fprintf(fout, "%s\n", aux.c_str());

   fprintf(fout, "ManAutoDef\n");
   fprintf(fout, "%6.2f;", pdef->ManAutoDef.tempo);
   fprintf(fout, "%10.6f;", pdef->ManAutoDef.ndef);
   fprintf(fout, "%i;", pdef->ManAutoDef.nBlocosManobra);
   fprintf(fout, "%6.2f;", pdef->ManAutoDef.clih);
   fprintf(fout, "%6.2f;", pdef->ManAutoDef.cli);
	aux = "-";
	if (pdef->ManAutoDef.chaveAbre != NULL)
		aux = pdef->ManAutoDef.chaveAbre->Codigo;
   fprintf(fout, "%s;", aux.c_str());
	aux = "-";
	if (pdef->ManAutoDef.chaveFecha != NULL)
		aux = pdef->ManAutoDef.chaveFecha->Codigo;
   fprintf(fout, "%s\n", aux.c_str());

   fprintf(fout, "ManProtDef\n");
   fprintf(fout, "%6.2f;", pdef->ManProtDef.tempo);
   fprintf(fout, "%10.6f;", pdef->ManProtDef.ndef);
   fprintf(fout, "%i;", pdef->ManProtDef.nBlocosManobra);
   fprintf(fout, "%6.2f;", pdef->ManProtDef.clih);
   fprintf(fout, "%6.2f;", pdef->ManProtDef.cli);
	aux = "-";
	if (pdef->ManProtDef.chaveAbre != NULL)
		aux = pdef->ManProtDef.chaveAbre->Codigo;
   fprintf(fout, "%s;", aux.c_str());
	aux = "-";
	if (pdef->ManProtDef.chaveFecha != NULL)
		aux = pdef->ManProtDef.chaveFecha->Codigo;
   fprintf(fout, "%s\n", aux.c_str());

   fprintf(fout, "ManReparo[24]\n");
   for(int n = 0; n < 24; n++)
	{
		fprintf(fout, "%6.2f;", pdef->ManReparo[n].tempo);
       fprintf(fout, "%10.6f;", pdef->ManReparo[n].ndef);
       fprintf(fout, "%i;", pdef->ManReparo[n].nBlocosManobra);
       fprintf(fout, "%6.2f;", pdef->ManReparo[n].clih);
       fprintf(fout, "%6.2f;", pdef->ManReparo[n].cli);
		aux = "-";
		if (pdef->ManReparo[n].chaveAbre != NULL)
			aux = pdef->ManReparo[n].chaveAbre->Codigo;
	   fprintf(fout, "%s;", aux.c_str());
		aux = "-";
		if (pdef->ManReparo[n].chaveFecha != NULL)
			aux = pdef->ManReparo[n].chaveFecha->Codigo;
	   fprintf(fout, "%s\n", aux.c_str());
      }

   fprintf(fout, "ManSocRapida[24]\n");
   for(int n = 0; n < 24; n++)
	{
		fprintf(fout, "%6.2f;", pdef->ManSocRapida[n].tempo);
       fprintf(fout, "%10.6f;", pdef->ManSocRapida[n].ndef);
       fprintf(fout, "%i;", pdef->ManSocRapida[n].nBlocosManobra);
       fprintf(fout, "%6.2f;", pdef->ManSocRapida[n].clih);
       fprintf(fout, "%6.2f;", pdef->ManSocRapida[n].cli);
		aux = "-";
		if (pdef->ManSocRapida[n].chaveAbre != NULL)
			aux = pdef->ManSocRapida[n].chaveAbre->Codigo;
	   fprintf(fout, "%s;", aux.c_str());
		aux = "-";
		if (pdef->ManSocRapida[n].chaveFecha != NULL)
			aux = pdef->ManSocRapida[n].chaveFecha->Codigo;
	   fprintf(fout, "%s\n", aux.c_str());
      }

   fprintf(fout, "ManSocLenta[24]\n");
   for(int n = 0; n < 24; n++)
	{
		fprintf(fout, "%6.2f;", pdef->ManSocLenta[n].tempo);
       fprintf(fout, "%10.6f;", pdef->ManSocLenta[n].ndef);
       fprintf(fout, "%i;", pdef->ManSocLenta[n].nBlocosManobra);
       fprintf(fout, "%6.2f;", pdef->ManSocLenta[n].clih);
       fprintf(fout, "%6.2f;", pdef->ManSocLenta[n].cli);
		aux = "-";
		if (pdef->ManSocLenta[n].chaveAbre != NULL)
			aux = pdef->ManSocLenta[n].chaveAbre->Codigo;
	   fprintf(fout, "%s;", aux.c_str());
		aux = "-";
		if (pdef->ManSocLenta[n].chaveFecha != NULL)
			aux = pdef->ManSocLenta[n].chaveFecha->Codigo;
	   fprintf(fout, "%s\n", aux.c_str());
      }

   fclose(fout);
   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::ImprimeParametrosDefeito(AnsiString arqSaida)
   {
   VTDefeito *pdef;
   FILE      *fout;
   TLisDefeitoRede *pDefeitoRede;
   VTBlocoRad *blocoRad;
   AnsiString aux;
   //VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   //AnsiString arq, aux;
   //arq = path->DirTmp() + "\\" + arqSaida;
   fout = fopen(arqSaida.c_str(), "wt");
   fprintf(fout, "-------------------------------------------------\n");
   fprintf(fout, "-------------Parametros Defeito  ----------------\n");
   fprintf(fout, "-------------------------------------------------\n");
   fprintf(fout, "CodRede;");
   fprintf(fout, "CodChave;");
   fprintf(fout, "CodChaveProt;");
   fprintf(fout, "religa;");
   fprintf(fout, "TaxaFalha;");
   fprintf(fout, "TReparo;");
   fprintf(fout, "ComprimentoBloco;");
   fprintf(fout, "NFalhas;");
   fprintf(fout, "TReparo;");
   fprintf(fout, "CLIH;");
   fprintf(fout, "CLI\n");
   for(int nr = 0; nr < lisDefeitoRede->Count; nr++)
      {
      pDefeitoRede = (TLisDefeitoRede*)lisDefeitoRede->Items[nr];
      for(int ndef = 0; ndef < pDefeitoRede->lisDefeitos->Count; ndef++)
         {
         pdef = (VTDefeito*)pDefeitoRede->lisDefeitos->Items[ndef];
		 fprintf(fout, "%s;", pDefeitoRede->rede->Codigo.c_str());
         blocoRad = buscaProt->IdentificaBlocoRad(pdef->Bloco);
         aux = "-";
         if(blocoRad != NULL)
			{
				if (blocoRad->Chave != NULL)
					aux = blocoRad->Chave->Codigo;
           }
		 fprintf(fout, "%s;", aux.c_str());
         aux = "-; - ";
         if(pdef->ChvProt != NULL)
			{
				aux = pdef->ChvProt->Codigo;
             if(buscaProt->ChaveReligadora(pdef->ChvProt) == true)
                aux.cat_sprintf(";RELIGA");
             else
                aux.cat_sprintf(";PROT");

            }
         fprintf(fout, "%s;", aux.c_str());

         fprintf(fout, "%6.2f;", pdef->TaxaFalha);
         fprintf(fout, "%6.2f;", pdef->TReparo);
         fprintf(fout, "%6.2f;", pdef->ComprimentoBloco);
         fprintf(fout, "%.6f;", pdef->NFalhas);
         fprintf(fout, "%.6f;", pdef->TReparo);
         fprintf(fout, "%.6f;", pdef->TCons/60);
         fprintf(fout, "%.6f\n", pdef->DCons);
         }
      }

   fclose(fout);
   }

//---------------------------------------------------------------------------
double __fastcall TCalcIndCont::FolgaIA(VTVisRede *visRede, int indPat)
   {
   return(visRede->FolgaIA(indPat));
   }

//---------------------------------------------------------------------------
VTDefeito* __fastcall TCalcIndCont::IdentificaDefeito(VTEqpto *eqpto)
   {
   VTBloco *bloco;
   VTDefeito *pdef = NULL;
   //Identifica bloco
   bloco = buscaProt->IdentificaBloco(eqpto);
   //Busca defeito
   for(int n = 0; n < lisDefeitos->Count; n++)
      {
      pdef = (VTDefeito*)lisDefeitos->Items[n];
		if (pdef->Bloco == bloco)
			break;
      pdef = NULL;
      }
   return(pdef);
   }

//---------------------------------------------------------------------------
VTChave* __fastcall TCalcIndCont::ChaveBloco(VTBloco *bloco)
	// hk201702: teste para execução de isolamentos no cálculo de indicadores: implementação de função que retorna a chave de suprimento do bloco
   {
   VTBlocoRad *blocoRad;
   blocoRad = buscaProt->IdentificaBlocoRad(bloco);
   if(blocoRad != NULL)
    return(blocoRad->Chave);
   else
    return(NULL);

   }


//---------------------------------------------------------------------------
int __fastcall TCalcIndCont::NCons(VTCarga *carga)
   {
   int naux  = 0;

   naux += carga->NumConsResidencial;
   naux += carga->NumConsComercial;
   naux += carga->NumConsIndustrial;
   naux += carga->NumConsRural;
   naux += carga->NumConsOutros; //MISTURADO COM IP!!!!
   naux += carga->NumConsA4;
   //Valor mínimo de consumidores
	if (naux == 0)
		naux = 1;

   return(naux);
   }

//---------------------------------------------------------------------------
double __fastcall TCalcIndCont::ComprimentoBloco(VTBloco *bloco)
   {
   TList *lisLiga = bloco->LisLigacao();
   VTEqpto *pliga;
   double    comp_km = 0.0;

   for(int n = 0; n < lisLiga->Count; n++)
      {
      pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
      comp_km += ((VTTrecho*)pliga)->Comprimento_km;
      }
   return(comp_km);
   }

//---------------------------------------------------------------------------
VTBlocos* __fastcall TCalcIndCont::PM_GetBlocos(void)
   {
   VTBlocos    *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   return(blocos);
   }

//---------------------------------------------------------------------------
VTDefeito* __fastcall TCalcIndCont::PM_GetDefeito(VTBloco *bloco)
   {
   VTDefeito *pdef = NULL;
   for(int n = 0; n < lisDefeitos->Count; n++)
      {
		if ((pdef = (VTDefeito*)lisDefeitos->Items[n])->Bloco == bloco)
			break;
      pdef = NULL;
      }
   return(pdef);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCalcIndCont::PM_GetLisBlocoRad(void)//hk201808: reformula: sai
   {
   return(buscaProt->LisBlocoRad);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCalcIndCont::PM_GetLisVisRede(void)//hk201808: reformula: sai
   {
   return(buscaProt->LisVisRede);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCalcIndCont::PM_GetLisChavesAtivas(void)
   {
   //Prepara a lista de chaves ativas com o último conjunto de chaves automáticas
   PreparaLisChavesAtivas();
   return(buscaProt->LisChavesAtivas);   //hk201808: reformula lisChavesAtivas
   }

//---------------------------------------------------------------------------
TList* __fastcall TCalcIndCont::PM_GetLisChavesAuto(void)//hk201808: reformula: sai
   {
   return(buscaProt->LisChvAuto);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCalcIndCont::PM_GetLisTipoEqProt(void)//hk201808: reformula: sai
   {
   return(buscaProt->LisTipoEqProt);
   }

//---------------------------------------------------------------------------
int    __fastcall TCalcIndCont::PM_GetIProfReliga(void)//hk201808: reformula: sai
   {
   return(buscaProt->IProfReliga);
   }

//---------------------------------------------------------------------------
void   __fastcall TCalcIndCont::PM_SetIProfReliga(int iProfReliga)//hk201808: reformula: sai
   {
   paramConf->ParamGerais->iProfReliga = iProfReliga;//hkedp
   buscaProt->IProfReliga = iProfReliga;
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ReiniciaLisParamRedeExterno(TList *lisParamRedeExt)//hkreformula: sai
   {
   TList *lisParamRede = paramConf->LisParamRede;
   strDadoParamRede *paramRedeExt, *paramRede;
   for(int np = 0; np < lisParamRedeExt->Count; np++)
      {
      paramRedeExt = (strDadoParamRede*)lisParamRedeExt->Items[np];
		if (paramRedeExt->rede == NULL)
			continue;
      //paramRede = ParametroConf(paramRedeExt->rede);
      paramRede = paramConf->ParamRede[paramRedeExt->rede];
      if(paramRede == NULL)
        lisParamRede->Add(paramRede = new strDadoParamRede);
      paramRede->rede    = paramRedeExt->rede;
      paramRede->codRede = paramRedeExt->codRede;
      paramRede->compRede= ComprimentoRede(paramRedeExt->rede);
      paramRede->lambda  = paramRedeExt->lambda;
      paramRede->trep    = paramRedeExt->trep; //taxa de falha e tempo de reparo
      //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
      paramRede->ptpre        = 0.0;
      paramRede->ptdlo        = 0.0;
      paramRede->ptexe        = 0.0;

      for(int nm = 0; nm < 12; nm++)
		{
			paramRede->nFEquip[nm] = paramRedeExt->nFEquip[nm];
          paramRede->durTot[nm]       = paramRedeExt->durTot[nm];
         }
      }
   //Calculo de paramRede
      //paramRede->nFalhasEquip
      //paramRede->durTotal
      //paramRede->ndiasObs
      //paramRede->nFEquip[nm]
      //paramRede->durTot[nm]
   //Funcao de
      //paramEqpto->tipoRede
      //paramEqpto->rede
      //paramEqpto->nFalhasEquip;
      //paramEqpto->durTotal;
      //paramEqpto->ndiasObs;

   //201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
	geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto, paramConf->LisParamRede,
		paramConf->ParamGerais);
	// ->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::ReiniciaLisParamEqptoExterno(TList *lisParamEqptoExt, bool flagInf) //hkreformula: sai
	// default bool flagInf = true
   {
   TList *lisParamEqpto = paramConf->LisParamEqpto;
   strDadoParamEqpto *paramEqpto, *paramEqptoExt;
   strDadoParamRede  *paramRede;

   //Reinicia ponteiro obj de cargas para busca de paramEqpto
   VTRedes       *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList         *lisCargas;
   VTCarga       *carga;
   lisCargas = new TList();
   redes->LisEqbar(lisCargas, eqptoCARGA);
   for(int nc = 0; nc < lisCargas->Count; nc++)
      {
      carga = (VTCarga*)lisCargas->Items[nc];
      carga->Obj = NULL;
      }
   delete lisCargas;
   //------------------------------------------------------------------
   //Reinicia informacoes de this->lisParamEqpto com lisParamEqptoExt
   //------------------------------------------------------------------
   if(flagInf == true) // utiliza apenas a informação e cria novos paramEqpto
     {
     for(int neq = 0; neq < lisParamEqptoExt->Count; neq++)
      {
      paramEqptoExt = (strDadoParamEqpto*)lisParamEqptoExt->Items[neq];
      //hk201808: reformula: paramEqpto    = ParametroDadoEqpto(paramEqptoExt->codEqpto, paramEqptoExt->codRede);
      paramEqpto    = paramConf->ParametroDadoEqpto(paramEqptoExt->codEqpto, paramEqptoExt->codRede);
			if (paramEqpto == NULL)
				lisParamEqpto->Add(paramEqpto = new strDadoParamEqpto);

      paramEqpto->paramOrig = (TObject*)paramEqptoExt;

      paramEqpto->rede           = paramEqptoExt->rede;
      paramEqpto->eqpto           = paramEqptoExt->eqpto;
      paramEqpto->codEqpto        = paramEqptoExt->codEqpto;
      paramEqpto->codRede        = paramEqptoExt->codRede;
      paramEqpto->carga           = paramEqptoExt->carga;

      //Atualiza o número de consumidores da carga
      if(paramEqpto->carga != NULL)  //Considera a carga concentrada
                {
                paramEqpto->nconsJus = NCons(paramEqpto->carga);
                paramEqpto->carga->Obj = paramEqpto;
                }

      paramEqpto->tipoRede        = paramEqptoExt->tipoRede;
      //hkpc paramEqpto->flagEncontrado= paramEqptoExt->flagEncontrado;
      paramEqpto->nFalhasEquip  = paramEqptoExt->nFalhasEquip;
      paramEqpto->durTotal        = paramEqptoExt->durTotal;
      paramEqpto->ndiasObs        = paramEqptoExt->ndiasObs;

        }
     }
   else
     {//Utiliza paramEqpto´s externos
     LimpaTList(lisParamEqpto);
     lisParamEqpto->Assign(lisParamEqptoExt, laOr);

     for(int neq = 0; neq < lisParamEqptoExt->Count; neq++)
      {
      paramEqptoExt = (strDadoParamEqpto*)lisParamEqptoExt->Items[neq];
			if (paramEqptoExt->carga != NULL)
				paramEqptoExt->carga->Obj = paramEqptoExt;
      }

     }

   //Calculo de paramRede
      //paramRede->nFalhasEquip
      //paramRede->durTotal
      //paramRede->ndiasObs
      //paramRede->nFEquip[nm]
      //paramRede->durTot[nm]
   //Funcao de
      //paramEqpto->tipoRede
      //paramEqpto->rede
      //paramEqpto->nFalhasEquip;
      //paramEqpto->durTotal;
      //paramEqpto->ndiasObs;
   //201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
	geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto, paramConf->LisParamRede,
		paramConf->ParamGerais);
	// ->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
   //Gera lisParamEqpto->paramEqpto->(lambda, trep) = f(paramEqpto->nFalhasEquip, clih, cli, ndiasObs)
   //Calcula
   //paramEqpto->lambda e paramEqpto->trep
   //em funcao de:
     //paramEqpto->eqpto
     //paramEqpto->carga
     //paramEqpto->clih, paramEqpto->cli - versão anterior: paramEqpto->durTotal
     //paramEqpto->nFalhasEquip
     //paramEqpto->ndiasObs
   geraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);

   //Reinicia lisParamBloco->paramBloco->(lambda, trep) e reconstroi lisParamCarga;
   //e lisParamCarga
   //Funcao de:
      //paramEqpto->eqpto
      //paramEqpto->rede
      //paramEqpto->lambda
      //paramEqpto->trep
   //Reconstroi lisParamCarga->strDadoParamCarga funcao de
   //    paramCarga->rede  = paramEqpto->rede;
   //  paramCarga->carga = paramEqpto->carga;
   //    paramCarga->lambda= paramEqpto->lambda;
   //    paramCarga->trep  = paramEqpto->trep;
   //    paramCarga->nconsJus = NCons(paramEqpto->carga);
   //Funcao de:
      //paramEqpto->carga
      //paramEqpto->rede
      //paramEqpto->lambda
      //paramEqpto->trep
   //Zera a lista de parametros por bloco
   //hkmarlimpa LimpaTList(paramConf->LisParamBloco);
	geraLisParamEqpto->RedistribuiFalhasCirc(paramConf);
	// hkmar (paramConf->LisParamEqpto, paramConf->LisParamBloco, paramConf->LisParamCarga);

   //Reinicia lisDefeito->pdef->(TaxaFalha, TReparo) = f(paramBloco, paramRede, paramDefault)
   IniciaParamDefeito();

   if(flagInf == true) // utiliza apenas a informação e cria novos paramEqpto
     {
     for(int neq = 0; neq < lisParamEqpto->Count; neq++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[neq];
      paramEqptoExt = (strDadoParamEqpto*)paramEqpto->paramOrig;
      paramEqptoExt->lambda = paramEqpto->lambda;
      paramEqptoExt->trep   = paramEqpto->trep;
      //hkpc paramEqptoExt->compSim= paramEqpto->compSim;
      }
     }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::LeArquivosMdb(AnsiString pathParam)//hkreformula: sai
   {
   bool resposta = true;
   AnsiString aux;
   if(pathParam != "")
       aux = ExtractFileDir(pathParam) + "\\";
   else
       aux = "";
   if(!paramConf->LeArquivosMdb(apl, aux)) return(false);
   PreparaParamConfRede(); //hkreformula: funcao complementar
   /*

   //VTBDadoParam *bdadoParam;
   //Atualiza path de paramConf.mdb
   //pathParam = ExtractFileDir(arqParam) + "\\";

   AnsiString arqParam, arqCircVerif, arqOcorrVerif;
   bool       resposta = true;

   if(pathParam.AnsiCompare("") == 0)
	{
		this->pathParam = LePathParam();
	} // leitura de tmp\dirConf.tmp
   else
	{
		this->pathParam = pathParam;
     }
   //Leitura de dados gerais
   arqParam = this->pathParam + "paramConf.mdb";

   //hkreformula
   if (!paramConf->LeArqMdb(apl, arqParam)) resposta = false;

	//if (!LeArqMdb(arqParam))
	//	resposta = false;
   */
   return(resposta);

   }

//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::InsereChaveIF(VTChave *pchave)//hkreformula
	{
   strDadoChaveIF* chvIF;
   //Verifica se já existe em paramConf->LisChavesIF
   if (paramConf != NULL)
		{
      chvIF = NULL;
      for(int nif = 0; nif < paramConf->LisChavesIF->Count; nif++)
         {
         chvIF = (strDadoChaveIF*)paramConf->LisChavesIF->Items[nif];
         if(chvIF->codChave.AnsiCompare(pchave->Codigo) == 0) continue;
         chvIF = NULL;
         }
      if(chvIF == NULL) paramConf->LisChavesIF->Add(chvIF = new strDadoChaveIF);
      chvIF->codChave = pchave->Codigo;
      chvIF->chave = pchave;
		}
   //Acrescenta chave em buscaProt->LisChavesIF->(chave)
   InsereLisChavesIF(paramConf->LisChavesIF);
	}
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::IniciaVerifDefault(void)//hkreformula: sai?
   {
   VTVerifCjto *verifCjto;
   VTRede      *rede;
   VTSubestacao *se;
   VTPrimario   *primario;
   VTVerifSe    *verifSe;
   TList *lisSE;
   TList *lisPRI;
   TList *lisVerifCjto;
   VTRedes  *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList    *lisRede;

   //if(paramConf->LisVerifCjto->Count > 0) return;
   //hk201701: Caso de um registro com parametros DEFAULT
	/*
	if (paramConf->LisVerifCjto->Count > 1)
		return;
	*/

   lisSE  = radial->LisSubestacao();
   lisPRI = radial->LisPrimario();

   //hk201701: Caso de um registro com parametros DEFAULT e sem lista de se
   /*
   if(paramConf->LisVerifCjto->Count == 1)
	 {
	 verifCjto = (VTVerifCjto*)paramConf->LisVerifCjto->Items[0];
	 if((verifCjto->Codigo.AnsiCompare("DEFAULT") == 0)&&(verifCjto->LisVerifSe->Count == 0))
	   {
	   lisRede = redes->LisRede();
	   for(int nr = 0; nr < lisRede->Count; nr++)
		  {
		  rede = (VTRede*)lisRede->Items[nr];
		  verifSe = paramConf->CriaVerifSe();
		  verifSe->Codigo = rede->Codigo;
		  verifCjto->LisVerifSe->Add(verifSe);
		  }
	   }
	 return;
	 }
   */

   //Cria um verifSe por subestacao
   for(int nse = 0; nse < lisSE->Count; nse++)
      {
      se = (VTSubestacao*)lisSE->Items[nse];
      verifCjto = paramConf->CriaVerifCjto();
      verifCjto->Codigo = se->Rede->Codigo;
      verifCjto->DecVerif = 10.0;
      verifCjto->FecVerif = 5.0;

      paramConf->LisVerifCjto->Add(verifCjto);
      paramConf->LisVerifSe->Add(verifSe = paramConf->CriaVerifSe());
      verifSe->Codigo = se->Rede->Codigo;
      verifCjto->LisVerifSe->Add(verifSe);
      }

	if (paramConf->LisVerifCjto->Count > 0)
		return;

   //Caso não haja SEs cria um parametro verificado para o conjunto
   //de primarios com codigo DEFAULT
   verifCjto = paramConf->CriaVerifCjto();
   verifCjto->Codigo = "DEFAULT";
   verifCjto->DecVerif = 10.0;
   verifCjto->FecVerif = 5.0;
   paramConf->LisVerifCjto->Add(verifCjto);
   for(int npri = 0; npri < lisPRI->Count; npri++)
      {
      primario = (VTPrimario*)lisPRI->Items[npri];
      paramConf->LisVerifSe->Add(verifSe = paramConf->CriaVerifSe());
      verifSe->Codigo = primario->Rede->Codigo;
      verifCjto->LisVerifSe->Add(verifSe);
      }

}

//---------------------------------------------------------------------------
double __fastcall TCalcIndCont::ComprimentoRede(VTRede *rede)//hkreformula: sai
   {
   TList *lisLiga;
   VTEqpto *pliga;
   double    comp_km = 0.0;

	if (rede == NULL)
		return (0.0);

   lisLiga = rede->LisLigacao();

   for(int n = 0; n < lisLiga->Count; n++)
      {
      pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
      comp_km += ((VTTrecho*)pliga)->Comprimento_km;
      }
   return(comp_km);
   }

//---------------------------------------------------------------------------
bool __fastcall TCalcIndCont::LisZonaProt(VTChave *pchv, TList *lisBlocoExt)//hk201808: funcao de buscaProt: tirar de calcIndCont
   {
   bool resposta;
   resposta = buscaProt->ListaZonaProt(pchv, lisBlocoExt);
   return(resposta);
   }

//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::GravaArqParam(AnsiString arqParam)//hkreformula: sai
//   {
   /*VTLeArqParamTxt *leArqParamTxt;

   leArqParamTxt = NewObjLeArqParamTxt(apl);

   leArqParamTxt->GravaArqParam(paramConf, pathParam);
   GravaDirConf();

   delete leArqParamTxt;
   return(true);
   */
//   }

//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::LeArqParam(AnsiString arqParam)  //hkreformula: sai
//   {
//   VTLeArqParamTxt *leArqParamTxt;
//
//   leArqParamTxt = NewObjLeArqParamTxt(apl);
//
//   leArqParamTxt->LeArqParam(paramConf, lisChavesClone, lisChavesNovas, arqParam);

   //Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramDefault->lambda;
   //--->--->pdef->TReparo   = paramDefault->trep;
//   IniciaParamDefeito();
//   delete leArqParamTxt;
//   return(true);
//   }

//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::GravaArqMdb(AnsiString arqParam)  //hk201808: reformula sai
//   {
//   paramConf->GravaArqMdb(apl, arqParam);
   /*hkreformula
	VTBDadoParam* bdadoParam;

	bdadoParam = DLL_NewObjBDadoParam(apl);
	//Abre arquivo mdb, arqParam
   bdadoParam->DefineCaminho(arqParam);
   //Grava as informações em arqParam
   bdadoParam->GravaDado(paramConf);
	delete bdadoParam;

   //Atualiza path de paramConf.mdb
   pathParam = ExtractFileDir(arqParam) + "\\";

   return(true);
   */
//   }

//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::BuscaBlocosAgreg(VTBloco *bloco, TList *lisExt)  //hk201808: reformula sai
   {
   //Gera para cada defeito os blocos agregados considerando chaves na rede mas nao ativadas
   /////////////////////////////////////////////////////////////////////////////////////////
   //Funcao que preenche lista com conjunto de blocos vizinhos a bloco ligados e com o//////
   //proprio bloco//////////////////////////////////////////////////////////////////////////
   //por chaves nao ativas//////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////
   buscaProt->BuscaBlocoAgreg(bloco, lisExt);
   }
*/
//---------------------------------------------------------------------------
//hk201808: sai: funcao de buscaProt
//void __fastcall TCalcIndCont::ListaOrdenadaBlocos(TList *lisExt, VTRede *rede)
//   {
//   buscaProt->ListaOrdenadaBlocos(lisExt, rede);
//   }
//---------------------------------------------------------------------------
/*strDadoParamRede* __fastcall TCalcIndCont::ParametroConfCodRede(AnsiString codRede)//hk201808: reformula: sai: funcao de paramConf: sai
   {
   //TList *lisParamRede = paramConf->LisParamRede;
   strDadoParamRede* paramRede = NULL;

   paramRede = paramConf->ParamRedeCod[codRede];

//   for(int np = 0; np < lisParamRede->Count; np++)
//      {
//      paramRede = (strDadoParamRede*)lisParamRede->Items[np];
//      if(paramRede->codRede.AnsiCompare(codRede.Trim()) == 0) break;
//      paramRede = NULL;
//      }
   return(paramRede);
   }
*/
//---------------------------------------------------------------------------
//strDadoParamEqpto* __fastcall TCalcIndCont::ParametroDadoEqpto(VTChave *chave) //hk201808: reformula: sai
//   {
//   return(paramConf->ParametroDadoEqpto(chave));
   /*
   TList *lisParamEqpto = paramConf->LisParamEqpto;
   strDadoParamEqpto *paramEqpto;
   //busca paramEqpto para o caso de rede BT representada por carga equivalente
   //do trafo de distribuição e da rede BT
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if (paramEqpto->eqpto == chave)
			break;
      paramEqpto = NULL;
      }
   return(paramEqpto);
   */
//   }

//---------------------------------------------------------------------------
//strDadoParamEqpto* __fastcall TCalcIndCont::ParametroDadoEqpto(AnsiString codChave,
//	AnsiString codRede) // hk201808: reformula: sai
//   {
//   paramConf->ParametroDadoEqpto(codChave, codRede);
   /*
   TList *lisParamEqpto = paramConf->LisParamEqpto;
   strDadoParamEqpto *paramEqpto;
   //busca paramEqpto
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if ((paramEqpto->codEqpto.AnsiCompare(codChave) == 0) && (paramEqpto->codRede.AnsiCompare
			(codRede) == 0))
		{
			break;
		}
      paramEqpto = NULL;
      }
   return(paramEqpto);
   */
//   }

//---------------------------------------------------------------------------
//strDadoParamEqpto* __fastcall TCalcIndCont::ParametroDadoEqpto(VTCarga *carga)//hk201808: reformula: sai
//   {
//   paramConf->ParametroDadoEqpto(carga);
   /*hkreformula: sai
   TList *lisParamEqpto = paramConf->LisParamEqpto;
   strDadoParamEqpto *paramEqpto;

   if(carga->Obj != NULL)
	{
		paramEqpto = (strDadoParamEqpto*)carga->Obj;
      return(paramEqpto);
     }

   //busca paramEqpto para o caso de rede BT representada por carga equivalente
   //do trafo de distribuição e da rede BT
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if (paramEqpto->carga == carga)
			break;
      paramEqpto = NULL;
      }
   return(paramEqpto);
   */
//   }

//---------------------------------------------------------------------------
//hk201808: funcao de buscaProt sai: tirar de calcIndCont
//void __fastcall TCalcIndCont::LisChavesOrd(TList *lisChavesOrd)
//   {
//   lisChavesOrd->Clear();
//   buscaProt->LisChaveOrd(lisChavesOrd);
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//bool __fastcall TCalcIndCont::LisZonaProtJus(VTChave *pchv, TList *lisBlocoExt)//hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
//   bool resposta;
//   resposta = buscaProt->ListaZonaProtJus(pchv, lisBlocoExt);
//   return(resposta);
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//bool __fastcall TCalcIndCont::LisBlProt(VTChave *pchv, TList *lisEXTBloco, TList *lisEXTChave)//hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
   //Preenche lisExtBloco com o blocos de pchv até chaves de protecao a jusante
//   return(buscaProt->LisBlProt(pchv, lisEXTBloco));
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//bool __fastcall TCalcIndCont::LisBlJus(VTChave *pchv, TList *lisEXTBloco)//hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
   //Retorna blocos a jusante da chave
//   return(buscaProt->LisBlJus(pchv, lisEXTBloco));
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//VTBarraRad* __fastcall TCalcIndCont::LisBarProt(VTBloco *bloco) //hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
   ////////////////////////////////////////////////////////////////////////////////////////
   //Cria lista de estruturas de barra pai -> barras filhas a partir da
   //chave de suprimento do bloco
   //com chave de suprimento blocoRad->pchave; blocoRad->bloco = bloco
   ////////////////////////////////////////////////////////////////////////////////////////
//   return(buscaProt->LisBarProt(bloco));
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//void __fastcall TCalcIndCont::LisChavesVis(VTRede *rede, TList *lisChaveExt)//hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
   ////////////////////////////////////////////////////////////////
   //Funcao que preenche lista de chaves das chaves NA ligadas   //
   //a rede até o suprimento da rede                             //
   ////////////////////////////////////////////////////////////////
//   buscaProt->LisChavesVis(rede, lisChaveExt);
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//void __fastcall TCalcIndCont::LisChavesProt(VTRede *rede, TList *lisChaveExt)//hk201808: funcao de buscaProt: tirar de calcIndCont
//   {
   /////////////////////////////////////////////////////////////////////////////////////
   //Funcao que preenche lista de chaves de todas as Chaves NF de protecao, exclusive,//
   //da rede até o suprimento                                                         //
   /////////////////////////////////////////////////////////////////////////////////////
//   buscaProt->LisChavesProt(rede, lisChaveExt);
//   }

//---------------------------------------------------------------------------
//hk201808: reformula: sai:
//VTVisRede* __fastcall TCalcIndCont::IdentificaVisRede(VTChave *pchv) //hk201808: reformula: sai
//   {
//   return(buscaProt->IdentificaVisRede(pchv));
//   }

//---------------------------------------------------------------------------
   //Inicia lisParamRede com parametros com valores default
   /*for(int nr = 0; nr < lisRede->Count; nr++)
      {
      rede = (VTRede*)lisRede->Items[nr];
      //Protecao
      if(!rede->Carregada) continue;
      if((paramRede = ParametroConf(rede)) != NULL) continue;
      lisParamRede->Add(paramRede = new strDadoParamRede);
      paramRede->codRede = rede->Codigo;
      paramRede->rede    = rede;
      //paramRede->flagParamVerif = false;
      //paramRede->flagHist       = false;
      paramRede->compRede = ComprimentoRede(rede);
      paramRede->lambda = 0.0;//paramDefault->lambda;
		paramRede->trep   = 0.0;;//paramDefault->trep;

      for(int nm = 0; nm < 12; nm++)
         {
         paramRede->nFEquip[nm] = 0.0;
         paramRede->durTot[nm] = 0.0;
         paramRede->lamb[nm] = 0.0;
         paramRede->tr[nm] = 0.0;
         }
      }
//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::CalculaCargaBlocos(VTRede *rede)
   {
   strResBloco *resBloco;
   //VTBarra  *barra;


   //------------------------------------------------------------
   //Calcula carga dos blocos e número de consumidores por bloco
   //blocoRad->CargaBloco[np] - carga de blocoRad->bloco
   //blocoRad->CargaBlRad[np] - carga do de blocoRad->bloco e blocos a jusante
   //------------------------------------------------------------
   buscaProt->CalculaCargasBlocoRad();

   //------------------------------------------------------------
   //Inicia objeto que armazena resultados
   //lisResRede->resRede
   //------------------------------------------------------------
   resCont->IniciaListas(rede);
   //Preenche ponteiro resBloco->blocoRad
   for(int n = 0; n < resCont->LisResBloco->Count; n++)
      {
      resBloco = (strResBloco*)resCont->LisResBloco->Items[n];
      resBloco->blocoRad = buscaProt->IdentificaBlocoRad(resBloco->bloco);
      //barra = (VTBarra*)(resBloco->bloco->LisBarra()->First());
      //barra->Obj = resBloco->blocoRad; //temporariamente
      }
   }
   */


//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::PM_GetFlagRecurso(void) //hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->flagRecurso);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetFlagRecurso(bool flagRecurso) //hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->flagRecurso = flagRecurso;
//   }

//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::PM_GetFlagRetorno(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->flagRetorno);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetFlagRetorno(bool flagRetorno)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->flagRetorno = flagRetorno;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetTLocaliza(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->tlocaliza);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetTLocaliza(double tlocaliza)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->tlocaliza = tlocaliza;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetTManManual(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->tman_manual);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetTManManual(double tman_manual)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->tman_manual = tman_manual;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetTManAuto(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->tman_auto);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetTManAuto(double tman_auto)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->tman_auto = tman_auto;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetTReparo(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->trep); //hkdef
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetTReparo(double trep)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->trep = trep;  //hkdef
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetLambda(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->lambda); //hkdef
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetLambda(double lambda)//hk201808: reformula: sai
//{
//	paramConf->ParamGerais->lambda = lambda; //hkdef
	/*if(IsDoubleZero(lambda))
	{
		paramConf->ParamGerais->lambda = 0.1;
	}
	else
	{
		paramConf->ParamGerais->lambda = lambda; //hkdef
	}*/
// }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetPuLim(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->pu_lim);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetPuLim(double pu_lim)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->pu_lim = pu_lim;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetPercDefTrans(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->percDefTrans);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetPercDefTrans(double percDefTrans)//hk201808: reformula: sai
//   {
//   paramConf->ParamGerais->percDefTrans = percDefTrans;
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetRedTprepIF(void)//hk201808: reformula: sai
//   {
//   return(paramConf->ParamGerais->red_tprep_IF);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetRedTprepIF(double red_tprep_IF)//hk201808: reformula: sai
//   {
//	paramConf->ParamGerais->red_tprep_IF = red_tprep_IF;
//   }


//---------------------------------------------------------------------------
/*
//hkreformula: vai para paramConf
void __fastcall TCalcIndCont::InsereLisTipoChaveProtecaoParamConf(TList *lisTipoEqProtExterno)
   {
   //Popula lista paramConf->LisTipoEqProt
   strDadoTipoEqProt *dadoTipoEqProt;
   VTTipoChave       *tipoChave;
   //Atualiza lista tipo de chaves em paramConf
   LimpaTList(paramConf->LisTipoEqProt);

   for(int n = 0; n < lisTipoEqProtExterno->Count; n++)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProtExterno->Items[n];
      paramConf->LisTipoEqProt->Add(dadoTipoEqProt = new strDadoTipoEqProt);
      dadoTipoEqProt->codTipoEqProt = tipoChave->Codigo;
      dadoTipoEqProt->tipoChave     = tipoChave;
      }

   }
*/
//---------------------------------------------------------------------------
/* //hk201808: reformula
void __fastcall TCalcIndCont::CalculaIndicadoresRedeBT(VTRede *rede, int iMes) //SAI201805
   {
   //Adiciona indicadores relativos a desligamentos por falhas BT
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   strDadoParamRede  *paramRede;
   strDadoParamCarga *paramCarga;
   strResRede  *resRede;
   double *distFalhasPat = paramConf->DistFalhasPat;
   TList       *lisParamCarga = paramConf->LisParamCarga;
   double        dicBT, ficBT, endBT;

   //FILE  *fout;
   //VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   //AnsiString arq;
   //arq = path->DirTmp()+"\\CalculaIndicadoresRedeBT.txt";
   //fout = fopen(arq.c_str(), "a");

   //soma indicadores por rede
   resRede  = resCont->ResRede[rede];
   //Zera indicadores relativos a falhas BT
   resRede->tdeslBTN    = 0;
   resRede->ndeslBTN    = 0;
   resRede->endBT       = 0;

   if(iMes < 0)
     {
     for(int n = 0; n < lisParamCarga->Count; n++)
        {
        paramCarga = (strDadoParamCarga*)lisParamCarga->Items[n];
			if (paramCarga->rede != rede)
				continue;

        //CalcDicFicBT
        dicBT = paramCarga->trep * paramCarga->lambda/60;
        ficBT = paramCarga->lambda;
        resRede->tdeslBTN += (dicBT * 60 * paramCarga->nconsJus);
        resRede->ndeslBTN += (ficBT * paramCarga->nconsJus);
        for(int np1 = 0; np1 < numPat; np1++)
           {
				resRede->endBT +=
					(distFalhasPat[np1] * dicBT * demanda->P_mw[paramCarga->carga][np1]);
           }
        //fprintf(fout, "%s\t %s\t %10.4f\t %10.4f\t %i\t %10.4f\t %10.4f\n", rede->Codigo, paramCarga->codCarga, paramCarga->lambda, paramCarga->trep, paramCarga->nconsJus, resRede->tdeslBTN, resRede->ndeslBTN);
        }
     }
   else
     {
     double lambda, trep;
     for(int n = 0; n < lisParamCarga->Count; n++)
        {
        paramCarga = (strDadoParamCarga*)lisParamCarga->Items[n];
			if (paramCarga->rede != rede)
				continue;
        //CalcDicFicBT
        lambda  = paramCarga->lamb_pu[iMes] * paramCarga->lambda;
        trep    = paramCarga->tr_pu[iMes]   * paramCarga->trep;
        dicBT = trep * lambda/60;
        ficBT = lambda;
        resRede->tdeslBTN += (dicBT * 60 * paramCarga->nconsJus);
        resRede->ndeslBTN += (ficBT * paramCarga->nconsJus);
        for(int np1 = 0; np1 < numPat; np1++)
           {
				resRede->endBT +=
					(distFalhasPat[np1] * dicBT * demanda->P_mw[paramCarga->carga][np1]);
           }
        }
     }

   resRede->tdeslN += resRede->tdeslBTN;
   resRede->ndeslN += resRede->ndeslBTN;
   resRede->end    += resRede->endBT;
   //fclose(fout);
   }
*/
//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::OrdenaVisRedeCap(VTDefeito *pdef, int iPat, bool flagAuto)
   {
   //Ordenacao de chaves por automação seguido da folga de transferencia
   //VTChave *pchave, *pchaveMax;
   VTVisRede *visRede, *visRedeMax;
   TList   *lisAux;
   double  capMax, capAux, capAuto;
   //Ordena pdef->LisChvVis por capacidade em iPat, buscaProt->FolgaIA(pchave, iPat)
   lisAux = new TList();

   lisAux->Assign(pdef->LisVisRedeDef, laOr);
   if(flagAuto)
     {
     capMax = -999.0;
     for(int n = 0; n < lisAux->Count; n++)
        {
        visRede = (VTVisRede*)lisAux->Items[n];
        //n14 capAux = buscaProt->FolgaIA(visRede, iPat);
        capAux = visRede->FolgaIA(iPat);
			if (capAux > capMax)
			{
				capMax = capAux;
        }
		}
     capAuto = capMax + 1;
     }

	if (capAuto < 0)
		capAuto = -capAuto;

   pdef->LisVisRedeDef->Clear();
   while(lisAux->Count > 0)
       {
       capMax = -999.0;
       for(int n = 0; n < lisAux->Count; n++)
          {
          visRede = (VTVisRede*)lisAux->Items[n];
          capAux = visRede->FolgaIA(iPat);
          //capAux = buscaProt->FolgaIA(visRede, iPat);
          if(flagAuto)
            {
				if (ChaveAutomatica(visRede->pchave))
					capAux += capAuto;
            }
			if (capAux > capMax)
			{
				visRedeMax = visRede;
				capMax = capAux;
          }
		}
       lisAux->Remove(visRedeMax);
       pdef->LisVisRedeDef->Add(visRedeMax);
       }
   delete lisAux;
   }
*/
//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::OrdenaVisRedeDeltaDEC(VTDefeito *pdef)
   {
   VTVisRede *visRede, *visRedeMin;
   //VTChave   *pchave, *pchaveMin;
   VTBlocoRad *blRad, *blRadJus;
   double     taux;
   int        index;
   TList   *lisAux;
   double  decAux, decMin;
   TList   *lisChvVis;
   //Ordenação por delta dec potencial

   lisChvVis = pdef->LisChvVisDef;

   for(int nv = 0; nv < pdef->LisVisRedeDef->Count; nv++)
      {
//      pchave = (VTChave*)LisChvVis->Items[nv];
//      if((visRede = buscaProt->IdentificaVisRede(pchave)) == NULL) continue;
      visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[nv];
      visRede->deltaDEC = 0.0;
      blRadJus = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Last();
      blRad = NULL;
      //Busca a primeira chave automática montante->jusante
      for(int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= 0; n--)
          {
          //BlocoRad a jusante do bloco de defeito
          blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];
			if (ChaveAutomatica(blRad->Chave))
			{
				index = n;
				break;
			}
          blRad = NULL;
          }
      if(blRad != NULL)
          {//Encontrou manobra rápida
           taux = paramConf->ParamGerais->tman_auto;
			if (ChaveAutomatica(visRede->pchave))
				taux += paramConf->ParamGerais->tman_auto;
			else
				taux += paramConf->ParamGerais->tman_manual;
           visRede->deltaDEC += (blRad->NCons * taux);
           //Manobra lenta
           if(index < visRede->lisBlocoRadIsolaJus->Count - 1)
             {
             taux = paramConf->ParamGerais->tman_auto + paramConf->ParamGerais->tman_manual;
             visRede->deltaDEC += (blRadJus->NCons - blRad->NCons) * taux;
             }
          }
      else
          {
           taux = paramConf->ParamGerais->tman_manual;
			if (ChaveAutomatica(visRede->pchave))
				taux += paramConf->ParamGerais->tman_auto;
			else
				taux += paramConf->ParamGerais->tman_manual;
           visRede->deltaDEC += blRadJus->NCons * taux;
          }

      }
   //Ordena alternativas de vis por deltaDEC

   lisAux = new TList();
   lisAux->Assign(pdef->LisVisRedeDef, laOr);
   pdef->LisVisRedeDef->Clear();
   while(lisAux->Count > 0)
       {
       //pchaveMin = (VTChave*)lisAux->First();
       //visRede = buscaProt->IdentificaVisRede(pchave);
       visRedeMin = (VTVisRede*)lisAux->First();
       decMin = visRede->deltaDEC;
       for(int n = 1; n < lisAux->Count; n++)
          {
          decAux = visRede->deltaDEC;
			if (decAux < decMin)
			{
				visRedeMin = visRede;
				decMin = decAux;
          }
		}
       lisAux->Remove(visRedeMin);
       pdef->LisVisRedeDef->Add(visRedeMin);
       }
   delete lisAux;
   }
*/
//---------------------------------------------------------------------------
//strDadoParamBloco* __fastcall TCalcIndCont::ParametroConfBloco(VTBloco *bloco)
//   {
//   return(paramConf->ParamBloco[bloco]);
   //return(paramConf->
   //TList *lisParamBloco = paramConf->LisParamBloco;
   //strDadoParamBloco *paramBloco = NULL;
   //for(int np = 0; np < lisParamBloco->Count; np++)
   //   {
   //   paramBloco = (strDadoParamBloco*)lisParamBloco->Items[np];
   //   if(paramBloco->bloco == bloco) break;
   //   paramBloco = NULL;
   //   }
   //return(paramBloco);
//   }
//---------------------------------------------------------------------------
//strDadoParamCarga* __fastcall TCalcIndCont::ParametroConfCarga(VTCarga *carga)
//   {
//   return(paramConf->ParamCarga[carga]);
   //TList *lisParamCarga = paramConf->LisParamCarga;
   //strDadoParamCarga *paramCarga = NULL;
   //for(int np = 0; np < lisParamCarga->Count; np++)
   //   {
   //   paramCarga = (strDadoParamCarga*)lisParamCarga->Items[np];
   //   if(paramCarga->carga == carga) break;
   //   paramCarga = NULL;
   //   }
   //return(paramCarga);
//   }

//---------------------------------------------------------------------------
//strDadoParamRede* __fastcall TCalcIndCont::ParametroConf(VTRede *rede)
//   {
//   return(paramConf->ParamRede[rede]);
   //TList *lisParamRede = paramConf->LisParamRede;
   //strDadoParamRede* paramRede = NULL;
   //if(rede == NULL) return(NULL);
   //for(int np = 0; np < lisParamRede->Count; np++)
   //   {
   //   paramRede = (strDadoParamRede*)lisParamRede->Items[np];
   //   if(paramRede->rede == rede) break;
   //   paramRede = NULL;
   //   }
   //return(paramRede);
//   }
//---------------------------------------------------------------------------
//strResBloco* __fastcall TCalcIndCont::PM_GetResBloco(VTBloco *bloco)
//   {
//   return(resCont->ResBloco[bloco]);
//   }

//---------------------------------------------------------------------------
//strResRede* __fastcall TCalcIndCont::PM_GetResRede(VTRede *rede)
//   {
//   return(resCont->ResRede[rede]);
//   }

//---------------------------------------------------------------------------
//strResRede* __fastcall TCalcIndCont::PM_GetResGeral(void)
//   {
//   return(resCont->ResGeral);
//   }

//---------------------------------------------------------------------------
//double __fastcall TCalcIndCont::PM_GetDistFalhasPat(int np)//hk201808: reformula: sai
//   {
//	if (!(np < numPat))
//		return (0.0);

//   return(paramConf->DistFalhasPat[np]);
   //return(distFalhasPat[np]);
//   }

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::PM_SetDistFalhasPat(int np, double dfalha)//hk201808: reformula: sai
//   {
//	if (!(np < numPat))
//		return;
//   paramConf->DistFalhasPat[np] = dfalha;
   //distFalhasPat[np] = dfalha;
//   }

//---------------------------------------------------------------------------
/*
void __fastcall TCalcIndCont::IniciaLisParamRede(void)  //hkreformula: sai
void __fastcall TCalcIndCont::IniciaLisVerifCjto(void)//hkreformula: sai
bool __fastcall TCalcIndCont::ReiniciaLisParamRedeExterno(TList *lisParamRedeExt)//hkreformula: sai
bool __fastcall TCalcIndCont::ReiniciaLisParamEqptoExterno(TList *lisParamEqptoExt, bool flagInf) //hkreformula: sai
bool __fastcall TCalcIndCont::LeArquivos(AnsiString pathParam) //hkreformula: sai
bool __fastcall TCalcIndCont::LeArquivosMdb(AnsiString pathParam)//hkreformula: sai
AnsiString __fastcall TCalcIndCont::LePathParam(void) //hkreformula: sai
bool __fastcall TCalcIndCont::LeArqMdb(AnsiString arqParam)//hkreformula: sai
void __fastcall TCalcIndCont::RedistribuiFalhasHora(void) //hkreformula: sai
void __fastcall TCalcIndCont::IniciaDistFalhas(void)//hkreformula: sai
void __fastcall TCalcIndCont::InsereChaveIF(VTChave *pchave)//hkreformula
bool __fastcall TCalcIndCont::FundeBlocos(TList *lisChavesFunde) //hkreformula: sai
void __fastcall TCalcIndCont::GravaDirConf(void)//hkreformula: vai para paramConf
void __fastcall TCalcIndCont::PreparaIndicacaoFalta(void)   //hk201710IF  //hk201808: reformulação: sai para buscaProt
void __fastcall TCalcIndCont::IniciaVerifDefault(void)//hkreformula: sai?
double __fastcall TCalcIndCont::ComprimentoRede(VTRede *rede)//hkreformula: sai
bool __fastcall TCalcIndCont::GravaArqParam(AnsiString arqParam)//hkreformula: sai
bool __fastcall TCalcIndCont::LeArqParam(AnsiString arqParam)  //hkreformula: sai
*/
//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::LeArqMdb(AnsiString arqParam)//hkreformula: sai
//   {
   //hk201808: reformula: sai
   /*
   VTBDadoParam *bdadoParam;

   //Atualiza path de paramConf.mdb
   pathParam = ExtractFileDir(arqParam) + "\\";

   //Cria objeto de leitura de paramConf.mdb
   bdadoParam = DLL_NewObjBDadoParam(apl);
   //Abre arquivo mdb definido, com path e nome: arqParam
   bdadoParam->DefineCaminho(arqParam);

   //Atualiza
   //paramConf->ParamGerais->flagRecurso, flagRetorno,tlocaliza,tman_manual,tman_auto,trep
   //                          lambda,pu_lim,percDefTrans,iProfReliga,tlimIC
   //--->LeDadoParamGerais();
   //Cria e preenche lisParamRede
   //param->codRede, param->lambda,param->trep
   //--->LeDadoParamRede();
   //Cria e preenche lisParamEqpto
   //param->codEqpto,codRede,tipoRede,ndiasObs,nf,nfr,td
   //param->nfmes[n=0,<12],nfrmes[n=0,<12],tdmes[n=0,<12]
   //param->nfhora[n=0,<24],nfrhora[n=0,<24]
   //--->LeDadoParamEqpto();
   //Cria e preenche lisParamBloco
   //param->codChave,lambda,trep
   //--->LeDadoParamBloco();
   //Cria e preenche lisParamCarga
   //param->codCarga,lambda,trep
   //--->LeDadoParamCarga();
   //Cria e preenche paramConf->LisVerifCjto
   //param->nomeSe,codSe,decVerif,fecVerif
   //--->LeDadoVerifCjto();
   //Cria e preenche paramConf->LisChavesTelecom
   //param->codChave
   //--->LeDadoChaveTelecom();
   //Cria e preenche paramConf->LisTipoEqProt
   //param->codTipoEqProt
   //     ->LeDadoTipoEqProt();

	//hkv201608 Reinicia parâmetros default e limpa listas
	//lisParamBloco;lisParamCarga;lisParamRede;lisParamEqpto;lisVerifCjto;lisChavesTelecom;lisTipoEqProt;
   paramConf->IniciaParamDefault();

   //Preenche dados gerais e listas
   //1.inicializa parâmetros gerais
   //ParamGerais->flagRecurso,ParamGerais->flagRetorno,ParamGerais->tlocaliza,
   //ParamGerais->tman_manual,ParamGerais->tman_auto
   //ParamGerais->pu_lim,   ParamGerais->percDefTrans,   ParamGerais->iProfReliga
   //ParamGerais->tlimIC,   ParamGerais->lambda,   ParamGerais->trep
   //2.lê ParamRede, atualiza paramConf->LisParamRede param->codRede, param->lambda, param->trep
   //Cria e popula dados relativos a ocorrencias médias por circuito
   //codRede,lambda,trep
   //2.lê ParamRede, atualiza paramConf->LisParamRede param->codRede, param->lambda, param->trep
   //Cria e popula dados relativos a ocorrencias médias por circuito
   //codRede,lambda,trep
   //3.lê ParamEqpto: limpa e atualiza paramConf->LisParamEqpto, para as redes de lisCodRede
   //Cria e popula dados relativos a equipamentos atuados na rede
   //codEqpto,codRede,tipoRede,
   //ndiasObs,nf,nfr,td,cli,clih,
   //nfmes[0-12], nfrmes[0-12],tdmes[0-12],climes[0-12],clihmes[0-12],
   //nfhora[0-24],nfrhora[0-24]
   //4.lê ParamBloco: limpa e atualiza paramConf->LisParamBloco
   //Cria e popula dados ajustados relativos a blocos, identiicados por código da chave de suprimento
   //codChave,lambda,trep
   //5.lê ParamCarga: limpa e atualiza paramConf->LisParamCarga
   //Cria e popula dados ajustados relativos a cargas, identiicados por código do trafo de rede MT ou cargaA4
   //codCarga,lambda,trep
   //6lê VerifCircuito : limpa e atualiza paramConf->LisVerifCircuito->VTVerifCircuito->(CodCirc, decVerif, fecVerif)
   //7.lê VerifSe       : limpa e paramConf->LisVerifSe->VTVerifSe->(CodCirc, decVerif, fecVerif)
   //8.lê VerifCjto     : limpa e paramConf->LisVerifCjto->VTVerifCjto->(CodCjto, decVerif, fecVerif) e popula verifCjto->LisVerifSe
   //9.lê ChaveTelecom  : limpa e atualiza paramConf->LisChavesTelecom
   //10.lê TipoEqProt    : limpa e atualiza paramConf->LisTipoEqProt

   //Define lista de códigos de redes carregadas
   TStringList *lisCodRede;
   VTRedes     *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede      *rede;
   TList       *lisRede;
   lisCodRede = new TStringList();
   lisRede = redes->LisRede();
   for(int n = 0; n < lisRede->Count; n++)
      {
      rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue;
      //Verifica se a rede corresponde a circuito primário, MT //hkv201608
      //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
      //if(!radial->ExistePrimario(rede)) continue;
      lisCodRede->Add(rede->Codigo);
      }
   //Popula em memória paramConf para as redes carregadas
   //a partir das tabelas de paramconf.mdb
   bdadoParam->LeDado(paramConf, lisCodRede);

   delete lisCodRede;
   delete bdadoParam;

   //1.PARAMCIRC atribui ponteiro de rede
   //paramRede->rede, paramRede->compRede
   //2.atribui ponteiros a lisParamEqpto, eqpto/ carga e gera distribuição de falhas
   //e gera paramEqpto->nfalhasEquip, durTotal
   //                 ->nfEquip[nm], durTot[nm]
   //3.Redistribui falhas para somar 1.0, soma distFalhasPat[0->23] = 1.0
   //4.Atribui ponteiro paramBloco->eqpto e paramBloco->bloco, paramBloco->compBloco
   //5.Atribui ponteiro paramCarga->rede (MT), paramCarga->carga  = carga e paramCarga->nconsJus = NCons(carga);
   //6.PARAMCHAVESTELECOM atribui ponteiro chvTelecom->chave de chave existente na rede
   //7.TIPOEQPROT atribui ponteiros dadoTipoEqProt->tipoChave de tipos de chave
   geraLisParamEqpto->PreparaParamConfRede(paramConf, NULL);
   //Seta em buscaProt variável de profundidade de religamento
   buscaProt->IProfReliga = paramConf->ParamGerais->iProfReliga;
   //Insere em buscaProt->LisTipoEqProt equipamentos de protecao de paramConf->LisTipoEqProt
   InsereLisTipoChaveProtecaoBuscaProt(paramConf->LisTipoEqProt);
   //Insere chaves telecomandadas -->reinicia buscaProt->LisChvAuto
   //e reinicia blocos agregados por chaves novas, pdef->LisBlocosAgrega
	InsereLisChavesTelecom(paramConf->LisChavesTelecom);
   //Reinicia e Atualiza buscaProt->LisChavesIF com paramConf->LisChavesIF
	InsereLisChavesIF(paramConf->LisChavesIF);

	//double* distFalhas = paramConf->DistFalhasPat;
   //for(int n = 0; n < 24; n++) distFalhasPat[n] = distFalhas[n];

   //hkv201608a Reinicia parametros associados a redes não incluídas na base
   //paramConf, mas carregadas na plataforma SINAP,
   //com valor de taxas e tempos de paramGeral
   paramConf->IniciaLisParamRede(redes->LisRede());//hkreformula: IniciaLisParamRede();

   //hkv201608a Caso não haja observações no banco de parametros inicia lisVerifCjto
   //com parametros default
   //hk201701: Caso de um registro com parametros DEFAULT para ajuste de parâmetros cria verificado com todas as redes
	//if (paramConf->LisVerifCjto->Count < 2)
	//hk201802a: IniciaVerifDefault();
   //if(paramConf->LisVerifCjto->Count == 0) IniciaVerifDefault();

   //Calculo de paramRede
      //paramGeral\paramRede->nFalhasEquip
      //paramGeral\paramRede->durTotal
      //paramGeral\paramRede->ndiasObs
      //paramGeral\paramRede->nFEquip[nm]
      //paramGeral\paramRede->durTot[nm]
      //paramGeral\paramRede->cli, clih
      //paramGeral\paramRede->climes[nm], clihmes[nm]
   //Funcao de
      //paramEqpto->tipoRede
      //paramEqpto->rede
      //paramEqpto->nFalhasEquip;
      //paramEqpto->durTotal;
      //paramEqpto->ndiasObs;
      //paramEqpto->nFEquip[nm]
      //paramEqpto->durTot[nm]
      //paramEqpto->cli, clih
      //paramEqpto->climes[nm], clihmes[nm]
   //201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
	geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto, paramConf->LisParamRede,
		paramConf->ParamGerais);
	// ->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1

   //Reinicia taxas e tempos de reparo lisParamEqpto->paramEqpto(lambda, trep)
   //Funcao de:
   //paramEqpto->clih, paramEqpto->cli - versão anterior: paramEqpto->durTotal
   //paramEqpto->nFalhasEquip
   //paramEqpto->ndiasObs
   geraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);

   //Reinicia parametros de defeito com taxas lidas da base de parametros, paramConf.mdb
   //Atualiza baseado em lisParamBloco, lisParamRede, paramGeral:
   //lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
   //Atualiza taxas (por blocos-->por circuito-->valor geral
   //pdef->TaxaFalha         = paramBloco->lambda;
   //pdef->TReparo           = paramBloco->trep;
   //--->pdef->TaxaFalha     = paramRede->lambda;
   //--->pdef->TReparo       = paramRede->trep;
   //--->--->pdef->TaxaFalha = paramGeral->lambda;
   //--->--->pdef->TReparo   = paramGeral->trep;
   IniciaParamDefeito();

   //Gera dic e fic pu_mensal com dados observados, flaMensal: existem 12 meses com observação
   //flagForcaAnual = true; //hktemp
   if(!flagForcaAnual) //hk201703: forca o calculo simplificado de compensacao
      flagMensal = ExecutaDicFicPuMensal();


   //VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov
   //FILE      *fout;
   //AnsiString arq;
   //arq = path->DirTmp() + "\\ParamSimula_LeArqMdb.txt";
   //paramConf->ImprimeParametrosSimula(arq);
   //Reinicia DICpu FICpu

   return(true);
   */
//   }

//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::IniciaLisParamRede(void)  //hk201808: reformula: sai
   {
   //Funcao que adiciona paramRede em paramConf->LisParamRede
   //para redes até o momento não inseridas
   //Utilizada em LeArqMdb(AnsiString arqParam)
   //          em IniciaProblema(VTRede *rede)
   VTRedes *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede  *rede;
   TList   *lisRede = redes->LisRede();
   strDadoParamRede *paramRede;

   for(int n = 0; n < lisRede->Count; n++)
      {
      rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue;
      //Verifica se a rede corresponde a circuito primário, MT //hkv201608
      //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
      //if(!radial->ExistePrimario(rede)) continue;
		if ((paramRede = paramConf->ParamRede[rede]) != NULL)
			continue;

      paramConf->LisParamRede->Add(paramRede = new strDadoParamRede);
      paramRede->rede = rede;
      paramRede->compRede = ComprimentoRede(paramRede->rede);
      paramRede->codRede = rede->Codigo;
      //hkv201607: novas redes inicializadas com paramGerais
      paramRede->lambda  = paramConf->ParamGerais->lambda;//0.1;//0.0;
      paramRede->trep    = paramConf->ParamGerais->trep;//50.0;//0.0;
      //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
      paramRede->ptpre   = 0.0;
      paramRede->ptdlo   = 0.0;
      paramRede->ptexe   = 0.0;
      }

   }

*/
//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::IniciaLisVerifCjto(void)//hk201808: reformula: sai
   {
   TList *lisSE = radial->LisSubestacao();
   //hkverifcjto strDadoVerifCjto *param;
   VTVerifCjto      *verifCjto;
   VTVerifSe        *verifSe;
   VTSubestacao     *se;
   //cria objeto
   for(int nse = 0; nse < lisSE->Count; nse++)
      {
      se = (VTSubestacao*)lisSE->Items[nse];
      //hkverifcjto
      //paramConf->LisVerifCjto->Add(param = new strDadoVerifCjto);
	  paramConf->LisVerifCjto->Add(verifCjto = paramConf->CriaVerifCjto());

	  //inicializa parâmetros
	  verifCjto->Codigo = se->Rede->Codigo;
      verifCjto->LisVerifSe->Add(verifSe = paramConf->CriaVerifSe());
	  verifSe->Codigo   = se->Rede->Codigo;
	  verifSe->CodigoCjto = verifCjto->Codigo;
      verifCjto->DecVerif = 10.0;
	  verifCjto->FecVerif = 5.0;

	  paramConf->LisVerifSe->Add(verifSe);

      //param->nomeSe   = se->Rede->Codigo;
      //param->codSe    = se->Rede->Codigo;
      //param->decVerif = 10.0;
      //param->fecVerif = 5.0;
      }
   }
*/

//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::ReiniciaDadosParamGeral(void)
//   {
//   geraLisParamEqpto->ReiniciaDadosParamGeral(paramConf->ParamGerais);
//   }
//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::ReiniciaDadosParamRede(void)
//   {
//   geraLisParamEqpto->ReiniciaDadosParamRede(paramConf->LisParamRede);
//   }
//---------------------------------------------------------------------------
//bool __fastcall TCalcIndCont::LeArquivos(AnsiString pathParam) //hkreformula: sai
//   {
/* //hk201808: reformula: função desativada
   AnsiString arqParam, arqCircVerif, arqOcorrVerif;
   bool       resposta = true;

   if(pathParam.AnsiCompare("") == 0)
	{
		this->pathParam = LePathParam();
	} // leitura de tmp\dirConf.tmp
   else
	{
		this->pathParam = pathParam;
     }
   //Leitura de dados gerais
   arqParam = this->pathParam + "parConfRec.txt";
	if (!LeArqParam(arqParam))
		resposta = false;

   return(resposta);
*/
//   }
//---------------------------------------------------------------------------
/*AnsiString __fastcall TCalcIndCont::LePathParam(void) //hk201808: reformula: sai
   {
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arqParam, dirConf;
   FILE    *fparam;
   bool    flagEOF;

   arqParam = path->DirTmp() + "\\dirConf.tmp";
	if (!FileExists(arqParam))
		return (path->DirDat() + "\\Confiabilidade\\"); // return(path->DirTmp() + "\\");

   fparam = fopen(arqParam.c_str(), "r");
   dirConf = "";
   //dirConf = path->DirDat();
   dirConf.cat_sprintf("%s", LeDado(fparam, flagEOF));
   fclose(fparam);

	if (!DirectoryExists(dirConf))
		dirConf = path->DirDat() + "\\Confiabilidade\\";
   //if(!DirectoryExists(dirConf)) dirConf = path->DirTmp() + "\\";
   return(dirConf);
   }
*/
//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::RedistribuiFalhasHora(void) //hkreformula: sai
   {
   double *distFalhasPat = paramConf->DistFalhasPat;

   double dTot = 0.0;

   for(int np = 0; np < numPat; np++)
      dTot += distFalhasPat[np];

   if(IsDoubleZero(dTot))
     {
     for(int np = 0; np < numPat; np++)
         distFalhasPat[np] = 1.0 / numPat;
     }
   else
     {
     for(int np = 0; np < numPat; np++)
         distFalhasPat[np] = distFalhasPat[np] / dTot;
     }
   }
*/
//---------------------------------------------------------------------------
/*void __fastcall TCalcIndCont::IniciaDistFalhas(void)//hk201808: reformula: sai
   {
   double *distFalhasPat = paramConf->DistFalhasPat;

	if (distFalhasPat != NULL)
		delete[]distFalhasPat;
   distFalhasPat = new double[numPat];
   for(int np = 0; np < numPat; np++)
                {
                distFalhasPat[np] = 1.0/numPat;
                }
   }
*/

//---------------------------------------------------------------------------
/*bool __fastcall TCalcIndCont::FundeBlocos(TList *lisChavesFunde) //hk201808: reformula: sai
   {
   VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTRedes  *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   bool    flagFunde;
   TList   *lisBlocos, *lisElo, *lisBlocosFunde, *lisEloT;
   VTBloco *bloco;
   VTElo   *elo;
   VTChave *chave;
   AnsiString aux;
   //protecao
	if (lisChavesFunde == NULL)
		return (false);

   //Executa blocos
   blocos->Executa(redes);

   lisBlocos = blocos->LisBloco();
   lisElo = new TList();
   lisBlocosFunde = new TList();
   flagFunde = true;
   while(flagFunde)
        {
        flagFunde = false;
        for(int nb = 0; nb < lisBlocos->Count; nb++)
           {
           bloco = (VTBloco*)lisBlocos->Items[nb];
           lisElo->Clear();
           blocos->LisElo(bloco, lisElo);
           lisBlocosFunde->Clear();
           for(int nelo = 0; nelo < lisElo->Count; nelo++)
              {
              elo   = (VTElo*)lisElo->Items[nelo];
              chave = elo->Chave;
				if (chave->Aberta)
					continue; // if(chave->Estado == chvABERTA) continue; //hkRevAloca
              if(lisChavesFunde->IndexOf(chave) == -1)
                 {
					if (elo->Bloco2 == bloco)
						lisBlocosFunde->Add(elo->Bloco1);
					else
						lisBlocosFunde->Add(elo->Bloco2);
                 }
              }
           if(lisBlocosFunde->Count > 0)
			{
				blocos->FundeBlocos(bloco, lisBlocosFunde);
               flagFunde = true;
               break;
              }
           }
        }
   //Retira e deleta elos de chaves NA fora de lisChvAuto
   lisEloT = blocos->LisElo();
   for(int nelo = lisEloT->Count - 1; nelo >= 0; nelo--)
      {
      elo   = (VTElo*)lisEloT->Items[nelo];
      chave = elo->Chave;
		if (!chave->Aberta)
			continue; // if(chave->Estado != chvABERTA) continue;  //hkRevAloca
		if (lisChavesFunde->IndexOf(chave) != -1)
			continue;
      lisEloT->Delete(nelo);
      delete elo;
      }

   delete lisElo;
   delete lisBlocosFunde;
   return(true);
   }
*/
//---------------------------------------------------------------------------
//void __fastcall TCalcIndCont::GravaDirConf(void)//hkreformula: vai para paramConf
//   {
//   paramConf->GravaDirConf(apl);
   /*//hkreformula
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq;
   FILE       *fmed;
   //Grava arquivo com diretório para o próximo acesso
   arq = path->DirTmp() + "\\dirConf.tmp";
   fmed = fopen(arq.c_str(), "wt");
   fprintf(fmed, "%s", pathParam);
   fclose(fmed);
   */
//   }

/*//---------------------------------------------------------------------------
void __fastcall TCalcIndCont::PreparaIndicacaoFalta(void)   //hk201710IF  //hk201808: reformulação: sai para buscaProt
   {
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList *lisChavesProt, *lisChavesProtMont;
   VTChave *pchv, *pchvProt;
   TList   *lisBlocosProtJus;
   VTBloco *bloco;
   double  compBloco;
   strIF   *infIF, *infIFProt;
   int     index;

   //Funcao que gera, por chave de protecao e chave com identif. de falta, IF
   //infIF = (chave, compProt, compIF)

   //Limpa a lista
   for(int n = 0; n < buscaProt->LisEQIF->Count; n++)
      {
      delete ((strIF*)buscaProt->LisEQIF->Items[n])->lisBlJus;
      delete ((strIF*)buscaProt->LisEQIF->Items[n])->lisBlProt;
      }
   LimpaTList(buscaProt->LisEQIF);

   lisChavesProt= new TList();
   redes->LisEqpto(lisChavesProt, eqptoCHAVE);
   //Cria lista de chaves de protecao
   for(int n = lisChavesProt->Count - 1; n >= 0; n--)
      {
      pchv = (VTChave*)lisChavesProt->Items[n];
      if(buscaProt->LisTipoEqProt->IndexOf(pchv->TipoChave) != -1) continue;
      lisChavesProt->Delete(n);
      }

   lisBlocosProtJus = new TList();
   for(int n = 0; n < lisChavesProt->Count; n++)
      {
      pchv = (VTChave*)lisChavesProt->Items[n];

      buscaProt->LisEQIF->Add(infIF = new strIF);
      infIF->lisBlJus = new TList();
      infIF->lisBlProt= new TList();

      if(buscaProt->LisChavesIF->IndexOf(pchv) != -1) infIF->flagIF = true;
      else infIF->flagIF = false;

      infIF->chave    = pchv;
      infIF->chaveProt= pchv;
      infIF->compProt = 0.0;
      infIF->compIF   = 0.0;
      //preenche buscaProt->LisEQIF para as chaves de protecao
      //1-Cálculo de compProt, distância total entre equipamentos de protecao
      buscaProt->ListaZonaProtJus(pchv, lisBlocosProtJus);
      infIF->lisBlProt->Assign(lisBlocosProtJus, laOr);
      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compProt += ComprimentoBloco(bloco);
         }


      //2-Cálculo de compIF, distância total entre equipamentos de protecao e chaves com IF, identif. de falta
      buscaProt->ListaZonaProtIFJus(pchv, lisBlocosProtJus);
      infIF->lisBlJus->Assign(lisBlocosProtJus, laOr);
      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compIF += ComprimentoBloco(bloco);
         }
      }

   lisChavesProtMont = new TList();
   for(int n = 0; n < buscaProt->LisChavesIF->Count; n++)
      {
		pchv = (VTChave*)buscaProt->LisChavesIF->Items[n];
      if(lisChavesProt->IndexOf(pchv) != -1) continue;

      buscaProt->LisEQIF->Add(infIF = new strIF);
      infIF->lisBlJus = new TList();
      infIF->lisBlProt= new TList();
      infIF->flagIF   = true;

      infIF->chave    = pchv;
      infIF->compProt = 0.0;
      infIF->compIF   = 0.0;
      //preenche buscaProt->LisEQIF para as chaves com IF, identificador de falta
      buscaProt->IdentificaChvMont(pchv, lisChavesProtMont, true);
      //proteca
      if(lisChavesProtMont->Count == 0) continue;
      pchvProt = (VTChave*)lisChavesProtMont->First();
      infIF->chaveProt= pchvProt;

      index = lisChavesProt->IndexOf(pchvProt);
      infIFProt = (strIF*)buscaProt->LisEQIF->Items[index];
      infIF->compProt = infIFProt->compProt;
      infIF->lisBlProt->Assign(infIFProt->lisBlProt, laOr);

      //2-Cálculo de compIF, distância total entre equipamentos de protecao e chaves com IF, identif. de falta
      buscaProt->ListaZonaProtIFJus(pchv, lisBlocosProtJus);
      infIF->lisBlJus->Assign(lisBlocosProtJus, laOr);

      for(int nb = 0; nb < lisBlocosProtJus->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocosProtJus->Items[nb];
         infIF->compIF += ComprimentoBloco(bloco);
         }
      }

   delete lisChavesProt;
   delete lisChavesProtMont;
   delete lisBlocosProtJus;
   }
*/

