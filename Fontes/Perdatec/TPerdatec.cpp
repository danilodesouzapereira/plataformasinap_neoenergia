//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPerdatec.h"
#include "VTBalancos.h"
#include "VTPerdas.h"
#include "VTResumos.h"
#include "RedePT\VTMontaRedePT.h"
#include ".\EnergiaPassante\VTEnergiaPassantes.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Potencia\VTdemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerdatec
//---------------------------------------------------------------------------
VTPerdatec* __fastcall NewObjPerdatec(VTApl *apl)
   {
   return(new TPerdatec(apl));
   }

//---------------------------------------------------------------------------
__fastcall TPerdatec::TPerdatec(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria lista
   lisGER = new TList();
   lisBAR_CLONE = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerdatec::~TPerdatec(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisGER) {delete lisGER; lisGER = NULL;}
   if (lisBAR_CLONE) {delete lisBAR_CLONE; lisBAR_CLONE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPerdatec::Clear(void)
   {
   //variáveis locais
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   montaRedePT->Clear();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
bool __fastcall TPerdatec::Executa(TDateTime data, int dia_semana)
   {
   //variáveis locais
   bool          incluir_rede_isolada = false;
   bool          sucesso              = false;  //DVK 2016.04.04 variável inicializada
   VTArvore      *arvore      = (VTArvore*)apl->GetObject(__classid(VTArvore));
   VTBalancos    *balancos    = (VTBalancos*)apl->GetObject(__classid(VTBalancos));
   VTEnergiaPassantes *energiaPass    = (VTEnergiaPassantes*)apl->GetObject(__classid(VTEnergiaPassantes));
   VTPerdas      *perdas      = (VTPerdas*)apl->GetObject(__classid(VTPerdas));
   VTRedes       *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTResumos     *resumos     = (VTResumos*)apl->GetObject(__classid(VTResumos));

   //insere/retira Eqptos na Rede para cálculo de perdas
   sucesso = PreparaRede(data);
   //executa cálculo de fluxo
   if (sucesso) sucesso = ExecutaCalculoFluxo(dia_semana);
   //monta Arvore c/ hierarquia das redes
   arvore->Data         = data;
   arvore->TipoArvore   = arvoreREDE;
   if (sucesso) sucesso = arvore->Inicia(redes->LisMRede());
   //salva Perdas
   if (sucesso) sucesso = perdas->Executa();
   //salva Balancos
   if (sucesso) sucesso = balancos->Executa();
   //salva energias passantes
   if (sucesso) sucesso = energiaPass->Executa();
   //remonta Arvore, substituindo as Redes pelos respectivos Resumos
   if (sucesso) sucesso = resumos->IniciaArvoreComResumo();
   //redefine o tipo da Arvore
   if (sucesso) arvore->TipoArvore = arvorePERDATEC;
   //reinicia objetos que contém dados que não são mais necessários
   perdas->Reinicia();
   balancos->Reinicia();
   if (energiaPass)
	{energiaPass->Reinicia();}

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdatec::ExecutaCalculoFluxo(int dia_semana)
   {
   //variáveis locais
   bool     dupla_precisao = false;
   VTCurvas *curvas        = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTFlow   *flow          = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGeral  *geral         = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTRedes  *redes         = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//verifica se as Curvas típicas são do mesmo dia da semana indicado
      if (curvas->DiaMedicao != dia_semana)
         {//seleciona curvas típicas do dia da semana indicado
         curvas->DiaMedicao = dia_semana;
         //reconfigura todas as cargas, associando-as com as curvas típicas selecionadas
         redes->RedefineCurvaTipicaCarga(curvas->DiaMedicao);
         }
      //configura opções do fluxo de potência
      flow->AvisoEnabled      = false;
      flow->CronometroEnabled = false;
      flow->TramoEnabled      = true;
      flow->TipoRelatorio     = relNENHUM;
      flow->ProgressoEnabled  = geral->CfgPerdaTec->Progresso;
      flow->TipoFluxo         = geral->CfgPerdaTec->TipoFluxo;
      flow->ModeloCarga       = geral->CfgPerdaTec->ModeloCarga;
      flow->PerdaFerroEnabled = (geral->CfgPerdaTec->FerroPorFora) ? false : true;
      //configura opção de fluxo alternativo no cálculo de cada rede
      flow->TipoFluxoAlt = fluxoEQUILIBRADO;
      flow->UpToDate = false;
      //executa cálculo por rede
      if (! flow->CalculaPorRede(redes, dupla_precisao)) return(false);
//      if (! flow->CalculaRedeUnica(redes)) return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   //sucesso
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdatec::PreparaRede(TDateTime data)
   {
   //variáveis locais
   bool           sucesso     = true;
   VTCurvas      *curvas      = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTDemanda     *demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTGeral       *geral       = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));
   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //define mês de referência de Curvas (para corrigir fator de correção de energia)
   curvas->DataReferencia = data;
   //define mês de referência da Demanda (para determinar dias do mês no cálculo da demanda média)
   demanda->DataReferencia = data;
   //verifica se houve alteração em CfgPerdaTec: teste importante quando se está rodando
   //o Perda Regulatória para evitar a chamada desnecessária de MontaRedePT
   if (geral->CfgPerdaTec->Alterada)
      {//insere/retira Eqptos na Rede para cálculo de perdas
      sucesso = montaRedePT->Executa();
      //indica que os parâmetros já foram usados
      geral->CfgPerdaTec->Alterada = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TPerdatec::ValidaRedeId(void)
   {
   //variáveis locais
   VTRede   *rede;
   VTRedes  *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList    *lisREDE = redes->LisRede();

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Id < 0) rede->Id = abs(FalsoId());
      }
   }


//---------------------------------------------------------------------------
//eof
