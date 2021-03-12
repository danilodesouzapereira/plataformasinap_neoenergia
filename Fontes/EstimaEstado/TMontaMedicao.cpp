//---------------------------------------------------------------------------
#pragma hdrstop
#include "TMontaMedicao.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaMedicao::TMontaMedicao(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria lista
   lisCHV = new TList();
   lisEQP = new TList();
   lisMED = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicao::~TMontaMedicao(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisMED) {delete lisMED; lisMED = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicao::Executa(bool por_fase)
   {
   //variáveis locais
   VTEdita   *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //retira Medidores atuais
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   if (lisEQP->Count > 0)
      {
      edita->RetiraLisEqpto(lisEQP);
      }
   //insere medidores
   InsereMedidor(por_fase);
   //atualiza o gráfico
   grafico->Redraw();

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMontaMedicao::IniciaLisChave(void)
   {
   //variáveis locais
   VTChave *chave;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de chaves da rede
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCHAVE);
   //reinicia lisCHV apenas com disjuntor e network protector
   lisCHV->Clear();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      chave = (VTChave*)lisEQP->Items[n];
      //não inclui Medição para chave de subestação
      if (chave->rede->TipoRede->Segmento == redeETD) continue;
      //inclui Medição para DJ, RA e Network Protector
      if ((chave->TipoChave->Tipo == chaveDJ) ||
          (chave->TipoChave->Tipo == chaveRA) ||
          (chave->TipoChave->Tipo == chaveNETWORK_PROTECTOR) ||
          (chave->TipoChave->Codigo.AnsiCompareIC("Network Protector") == 0))
         {
         lisCHV->Add(chave);
         }
      }
   return(lisCHV->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicao::InsereMedidor(bool por_fase)
   {
   //variáveis locais
   int       ind_bar = 0;
   int       ind_pat = 0;
   VTBarra   *barra;
   VTChave   *chave;
   VTMedidor *medidor;
   VTEdita   *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //monta lista de Chaves contendo disjuntor e  network rotector
   IniciaLisChave();
   //loop para todas Chaves em lisCHV
   for (int n = 0; n < lisCHV->Count; n++)
      {
      chave = (VTChave*)lisCHV->Items[n];
      //determina Barra para a qual a pot.ativa ativa é positiva
      if (chave->resflow->SentidoP[ind_bar][ind_pat] == fluxoNEG)
         {barra = chave->Barra(1 - ind_bar);}
      else
         {barra = chave->Barra(ind_bar);}
      //se necessário, cria um Medidor na Barra e salva em lisMED
      medidor = InsereMedidor(barra);
      //cria Canais P e Q para para a Chave
      if (por_fase) InsereCanaisPQ_porfase(medidor, chave);
      else          InsereCanaisPQ_trifasico(medidor, chave);
      }
   //insere todos os Medidores na rede
   edita->InsereLisEqpto(lisMED);
   return(true);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicao::InsereMedidor(VTBarra *barra, AnsiString codigo)
   {
   //variáveis locais
   VTMedidor *medidor;

   //verifica se já existe um Medidor na Barra
   for (int n = 0; n < lisMED->Count; n++)
      {
      medidor = (VTMedidor*)lisMED->Items[n];
      if (medidor->pbarra == barra) return(medidor);
      }
   //verifica se o código do Medidor foi definido
   if (codigo.IsEmpty()) codigo.sprintf("M.%03d", lisMED->Count+1);
   //cria Medidor e insere em lisMED
   lisMED->Add(medidor = DLL_NewObjMedidor());
   medidor->Codigo    = codigo;
   medidor->esq.dxbar = 50;
   medidor->esq.dybar = 50;
   //associa Medidor com sua Barra (sem inserir)
   medidor->pbarra = barra;

   return(medidor);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicao::InsereCanaisPQ_porfase(VTMedidor *medidor, VTChave* chave)
   {
   //variáveis locais
   struct{
         const char *codigo;
         int        ind_fase;
         int        fase_tag;
         }vet_canal[3] =  { {"a", indFASE_A, faseA},
                            {"b", indFASE_B, faseB},
                            {"c", indFASE_C, faseC} };
   int         ind_bar;
   double      tolerancia_perc;
   VTCanal     *canal;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //define tolerância em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   ind_bar = chave->IndiceBarra(medidor->pbarra);
   //insere canal P para as 3 fases
   for (int n = 0; n < 3; n++)
      {
      canal = InsereCanal_P(medidor, AnsiString("P") + vet_canal[n].codigo, chave, vet_canal[n].fase_tag, tolerancia_perc);
      //preenche os valores medidos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Pf_mw[ind_bar][ind_pat][n] * 1.05;
         }
      }
   //insere canal Q para as 3 fases
   for (int n = 0; n < 3; n++)
      {
      canal = InsereCanal_Q(medidor, AnsiString("Q") + vet_canal[n].codigo, chave, vet_canal[n].fase_tag, tolerancia_perc);
      //preenche os valores medidos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Qf_mvar[ind_bar][ind_pat][n] * 1.05;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicao::InsereCanaisPQ_trifasico(VTMedidor *medidor, VTChave* chave)
   {
   //variáveis locais
   int         ind_bar;
   double      tolerancia_perc;
   VTCanal     *canal;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //define tolerância em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   ind_bar = chave->IndiceBarra(medidor->pbarra);
   //insere canal P para faseABC
   canal = InsereCanal_P(medidor, AnsiString("Pabc"), chave, faseABC, tolerancia_perc);
   //preenche os valores medidos
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->P_mw[ind_bar][ind_pat] * 1.05;
      }
   //insere canal Q para faseABC
   canal = InsereCanal_Q(medidor, AnsiString("Qabc"), chave, faseABC, tolerancia_perc);
   //preenche os valores medidos
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Q_mvar[ind_bar][ind_pat] * 1.05;
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicao::InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalP, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicao::InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalQ, fases);
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicao::InsereCurva(VTCanal *canal)
   {
   try{//verifica se o Canal não possui Curva
      if (canal->Curva == NULL)
         {//cria Curva p/ a Canal
			canal->Curva                = DLL_NewObjCurva();
         canal->Curva->Tipica        = false;
         canal->Curva->NumeroValores = nvCURVA_CANAL;
         canal->Curva->Escala        = pat01HORA;
         switch(canal->TipoCanal)
            {
            case canalV:
			      canal->Curva->Tipo          = curvaV;
               canal->Curva->Unidade       = unidKV;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalI:
			      canal->Curva->Tipo          = curvaI;
               canal->Curva->Unidade       = unidA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalP:
			      canal->Curva->Tipo          = curvaP;
               canal->Curva->Unidade       = unidMW;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalQ:
			      canal->Curva->Tipo          = curvaQ;
               canal->Curva->Unidade       = unidMVAR;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalS:
			      canal->Curva->Tipo          = curvaS;
               canal->Curva->Unidade       = unidMVA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            default:
               break;
            }
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   return(canal->Curva != NULL);
   }

//---------------------------------------------------------------------------
//eof

