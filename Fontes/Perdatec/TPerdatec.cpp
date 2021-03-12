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
//fun��o global p/ criar objeto da classe VTPerdatec
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
   //destr�i lista sem destruir seus objetos
   if (lisGER) {delete lisGER; lisGER = NULL;}
   if (lisBAR_CLONE) {delete lisBAR_CLONE; lisBAR_CLONE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPerdatec::Clear(void)
   {
   //vari�veis locais
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   montaRedePT->Clear();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
bool __fastcall TPerdatec::Executa(TDateTime data, int dia_semana)
   {
   //vari�veis locais
   bool          incluir_rede_isolada = false;
   bool          sucesso              = false;  //DVK 2016.04.04 vari�vel inicializada
   VTArvore      *arvore      = (VTArvore*)apl->GetObject(__classid(VTArvore));
   VTBalancos    *balancos    = (VTBalancos*)apl->GetObject(__classid(VTBalancos));
   VTEnergiaPassantes *energiaPass    = (VTEnergiaPassantes*)apl->GetObject(__classid(VTEnergiaPassantes));
   VTPerdas      *perdas      = (VTPerdas*)apl->GetObject(__classid(VTPerdas));
   VTRedes       *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTResumos     *resumos     = (VTResumos*)apl->GetObject(__classid(VTResumos));

   //insere/retira Eqptos na Rede para c�lculo de perdas
   sucesso = PreparaRede(data);
   //executa c�lculo de fluxo
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
   //reinicia objetos que cont�m dados que n�o s�o mais necess�rios
   perdas->Reinicia();
   balancos->Reinicia();
   if (energiaPass)
	{energiaPass->Reinicia();}

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdatec::ExecutaCalculoFluxo(int dia_semana)
   {
   //vari�veis locais
   bool     dupla_precisao = false;
   VTCurvas *curvas        = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTFlow   *flow          = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGeral  *geral         = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTRedes  *redes         = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//verifica se as Curvas t�picas s�o do mesmo dia da semana indicado
      if (curvas->DiaMedicao != dia_semana)
         {//seleciona curvas t�picas do dia da semana indicado
         curvas->DiaMedicao = dia_semana;
         //reconfigura todas as cargas, associando-as com as curvas t�picas selecionadas
         redes->RedefineCurvaTipicaCarga(curvas->DiaMedicao);
         }
      //configura op��es do fluxo de pot�ncia
      flow->AvisoEnabled      = false;
      flow->CronometroEnabled = false;
      flow->TramoEnabled      = true;
      flow->TipoRelatorio     = relNENHUM;
      flow->ProgressoEnabled  = geral->CfgPerdaTec->Progresso;
      flow->TipoFluxo         = geral->CfgPerdaTec->TipoFluxo;
      flow->ModeloCarga       = geral->CfgPerdaTec->ModeloCarga;
      flow->PerdaFerroEnabled = (geral->CfgPerdaTec->FerroPorFora) ? false : true;
      //configura op��o de fluxo alternativo no c�lculo de cada rede
      flow->TipoFluxoAlt = fluxoEQUILIBRADO;
      flow->UpToDate = false;
      //executa c�lculo por rede
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
   //vari�veis locais
   bool           sucesso     = true;
   VTCurvas      *curvas      = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTDemanda     *demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTGeral       *geral       = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));
   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //define m�s de refer�ncia de Curvas (para corrigir fator de corre��o de energia)
   curvas->DataReferencia = data;
   //define m�s de refer�ncia da Demanda (para determinar dias do m�s no c�lculo da demanda m�dia)
   demanda->DataReferencia = data;
   //verifica se houve altera��o em CfgPerdaTec: teste importante quando se est� rodando
   //o Perda Regulat�ria para evitar a chamada desnecess�ria de MontaRedePT
   if (geral->CfgPerdaTec->Alterada)
      {//insere/retira Eqptos na Rede para c�lculo de perdas
      sucesso = montaRedePT->Executa();
      //indica que os par�metros j� foram usados
      geral->CfgPerdaTec->Alterada = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TPerdatec::ValidaRedeId(void)
   {
   //vari�veis locais
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
