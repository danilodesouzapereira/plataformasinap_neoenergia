//---------------------------------------------------------------------------
#pragma hdrstop
#include "TMontaMedicaoChave.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
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
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoChave::TMontaMedicaoChave(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria lista
   lisCHV     = new TList();
   lisEQP     = new TList();
   lisMED_ALL = new TList();
   lisMED_NEW = new TList();
   //habilita medição para DJ e RA
   DJ_Enabled = true;
   RA_Enabled = true;

   // Inicialização
   FatMultMedChave = 1.;
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoChave::~TMontaMedicaoChave(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisMED_ALL) {delete lisMED_ALL; lisMED_ALL = NULL;}
   if (lisMED_NEW) {delete lisMED_NEW; lisMED_NEW = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::Executa(int tipo_medicao, bool por_fase)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //insere medidores
   InsereMedidor(tipo_medicao, por_fase);
   //habilita/desabilita Canais associados a chaves DJ
   HabilitaDesabilitaCanais();
   //atualiza o gráfico
   grafico->Redraw();

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedicaoChave::HabilitaDesabilitaCanais(void)
   {
   //variáveis locais
   TList     *lisCANAL;
   VTMedidor *medidor;
   VTCanal   *canal;
   VTChave   *chave;

   //obtém lista de todos Medidores
   IniciaLisMedidor();
   //loop p/ todos Medidores
   for (int nm = 0; nm < lisMED_ALL->Count; nm++)
      {
      medidor = (VTMedidor*)lisMED_ALL->Items[nm];
      //loop p/ todos Canais do Medidor
      lisCANAL = medidor->LisCanal();
      for (int nc = 0; nc < lisCANAL->Count; nc++)
         {
         canal = (VTCanal*)lisCANAL->Items[nc];
         //verifica se Canal associado a uma Chave
         if (canal->Eqpto->Tipo() == eqptoCHAVE)
            {
            chave = (VTChave*)canal->Eqpto;
            if      (chave->TipoDisjuntor)  canal->Enabled = DJ_Enabled;
            else if (chave->TipoReligadora) canal->Enabled = RA_Enabled;
            }
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TMontaMedicaoChave::IniciaLisChave(void)
   {
   //variáveis locais
   VTBarra *barra;
   VTChave *chave;
   VTRede  *rede;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de chaves da rede
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCHAVE);
   //reinicia lisCHV apenas com disjuntor e network protector
   lisCHV->Clear();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      chave = (VTChave*)lisEQP->Items[n];
      rede  = chave->rede;
      barra = rede->BarraInicial();
      //não inclui Medicao para Chave aberta
      if (chave->Aberta) continue;
      //não inclui Medição para chave de subestação
      if (rede->TipoRede->Segmento == redeETD) continue;
      //inclui Medição para DJ da barra inicial
      if ((chave->TipoChave->Tipo == chaveDJ) && (chave->ConectaBarra(barra)))
         {
         lisCHV->Add(chave);
         }
      //inclui Medição para RA
      if (chave->TipoChave->Tipo == chaveRA)
         {
         lisCHV->Add(chave);
         }
      //inclui Medição para Network Protector
      //if (chave->TipoChave->Tipo == chaveNETWORK_PROTECTOR)
      if (chave->TipoChave->Codigo.AnsiCompareIC("Network Protector") == 0)
         {
         lisCHV->Add(chave);
         }
      }
   return(lisCHV->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TMontaMedicaoChave::IniciaLisMedidor(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Medidores da rede
   lisMED_ALL->Clear();
   redes->LisEqbar(lisMED_ALL, eqptoMEDIDOR);

   return(lisMED_ALL->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereMedidor(int tipo_medicao, bool por_fase)
   {
   //variáveis locais
   int       ind_bar = 0;
   int       ind_pat = 0;
   VTBarra   *barra;
   VTChave   *chave;
   VTMedidor *medidor;
   VTEdita   *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //inicia lisMED com os Medidores já existentes na Rede
   IniciaLisMedidor();
   //monta lista de Chaves contendo disjuntor, religador e  network rotector
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
      //verifica o tipo de medição
      if (tipo_medicao == medicaoI)
         {//cria canais I para a Chave
         if (por_fase) InsereCanaisI_porfase(medidor, chave);
         else          InsereCanaisI_trifasico(medidor, chave);
         }
      else if (tipo_medicao == medicaoPQ)
         {//cria Canais P e Q para para a Chave
         if (por_fase) InsereCanaisPQ_porfase(medidor, chave);
         else          InsereCanaisPQ_trifasico(medidor, chave);
         }
      else if (tipo_medicao == medicaoV)
         {//cria Canais P e Q para para a Chave
         InsereCanaisV_porfase(medidor, chave);
         }
      }
   //insere os Medidores criados na rede
   edita->InsereLisEqpto(lisMED_NEW);
   return(true);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicaoChave::InsereMedidor(VTBarra *barra, AnsiString codigo)
   {
   //variáveis locais
   VTMedidor *medidor;

   //verifica se já existe um Medidor na Barra
   for (int n = 0; n < lisMED_ALL->Count; n++)
      {
      medidor = (VTMedidor*)lisMED_ALL->Items[n];
      if (medidor->pbarra == barra) return(medidor);
      }
   //verifica se o código do Medidor foi definido
   if (codigo.IsEmpty()) codigo.sprintf("M.%03d", lisMED_ALL->Count+1);
   //cria Medidor e insere em lisMED_ALL e lisMED_NEW
   if ((medidor = DLL_NewObjMedidor()) != NULL)
      {
      lisMED_ALL->Add(medidor);
      lisMED_NEW->Add(medidor);
      medidor->Id        = FalsoId();
      medidor->Codigo    = codigo;
      medidor->esq.dxbar = 50;
      medidor->esq.dybar = 50;
      //associa Medidor com sua Barra (sem inserir)
      medidor->pbarra = barra;
      }
   return(medidor);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCanaisI_porfase(VTMedidor *medidor, VTChave* chave)
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

   //define tolerancia em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   ind_bar = chave->IndiceBarra(medidor->pbarra);
   //insere canal P para as 3 fases
   for (int n = 0; n < 3; n++)
      {
      canal = InsereCanal_I(medidor, AnsiString("I") + vet_canal[n].codigo, chave, vet_canal[n].fase_tag, tolerancia_perc);
      //preenche os valores medidos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         canal->ValorMedido[patamares->Patamar[ind_pat]] = Abs(chave->resflow->If_amp[ind_bar][ind_pat][n]) * FatMultMedChave;
         }
      //habilita/desabilita o Canal
      canal->Enabled = chave->Fechada;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCanaisI_trifasico(VTMedidor *medidor, VTChave* chave)
   {
   //variáveis locais
   int         ind_bar;
   double      tolerancia_perc;
   VTCanal     *canal;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //define tolerancia em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   ind_bar = chave->IndiceBarra(medidor->pbarra);
   //insere canal P para faseABC
   canal = InsereCanal_I(medidor, AnsiString("Iabc"), chave, faseABC, tolerancia_perc);
   //preenche os valores medidos
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->If_amp_max[ind_bar][ind_pat] * FatMultMedChave;
      }
   //habilita/desabilita o Canal
   canal->Enabled = chave->Fechada;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCanaisPQ_porfase(VTMedidor *medidor, VTChave* chave)
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

   //define tolerancia em função do tipo da chave
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
         canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Pf_mw[ind_bar][ind_pat][n] * FatMultMedChave;
         }
      //habilita/desabilita o Canal
      canal->Enabled = chave->Fechada;
      }
   //insere canal Q para as 3 fases
   for (int n = 0; n < 3; n++)
      {
      canal = InsereCanal_Q(medidor, AnsiString("Q") + vet_canal[n].codigo, chave, vet_canal[n].fase_tag, tolerancia_perc);
      //preenche os valores medidos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Qf_mvar[ind_bar][ind_pat][n] * FatMultMedChave;
         }
      //habilita/desabilita o Canal
      canal->Enabled = chave->Fechada;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCanaisPQ_trifasico(VTMedidor *medidor, VTChave* chave)
   {
   //variáveis locais
   int         ind_bar;
   double      tolerancia_perc;
   VTCanal     *canal;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //define tolerancia em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   ind_bar = chave->IndiceBarra(medidor->pbarra);
   //insere canal P para faseABC
   canal = InsereCanal_P(medidor, AnsiString("Pabc"), chave, faseABC, tolerancia_perc);
   //preenche os valores medidos
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->P_mw[ind_bar][ind_pat] * FatMultMedChave;
      }
   //insere canal Q para faseABC
   canal = InsereCanal_Q(medidor, AnsiString("Qabc"), chave, faseABC, tolerancia_perc);
   //preenche os valores medidos
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      canal->ValorMedido[patamares->Patamar[ind_pat]] = chave->resflow->Q_mvar[ind_bar][ind_pat] * FatMultMedChave;
      }
   //habilita/desabilita o Canal
   canal->Enabled = chave->Fechada;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCanaisV_porfase(VTMedidor *medidor, VTChave* chave)
   {
   //variáveis locais
   struct{
         const char *codigo;
         int        ind_fase;
         int        fase_tag;
         }vet_canal[3] =  { {"a", indFASE_A, faseA},
                            {"b", indFASE_B, faseB},
                            {"c", indFASE_C, faseC} };
   double      tolerancia_perc;
   VTCanal     *canal;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //define tolerancia em função do tipo da chave
   if (chave->TipoChave->Tipo == chaveDJ) tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
   else                                   tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
   //determina indice da Barra de referência da Chave
   //insere canal V para as 3 fases
   for (int n = 0; n < 3; n++)
      {
      canal = InsereCanal_V(medidor, AnsiString("V") + vet_canal[n].codigo, chave, vet_canal[n].fase_tag, tolerancia_perc);
      //preenche os valores medidos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         canal->ValorMedido[patamares->Patamar[ind_pat]] = Abs(medidor->pbarra->resflow->Vfn_pu[ind_pat][n]);
         }
      }
   //habilita/desabilita o Canal
   canal->Enabled = chave->Fechada;
   return(true);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoChave::InsereCanal_I(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe o Canal
   if ((canal = medidor->ExisteCanal(eqpto, canalI, fases)) == NULL)
      {//cria um novo Canal
      canal = medidor->InsereCanal(eqpto, canalI, fases);
      }
   canal->Id              = FalsoId();
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoChave::InsereCanal_P(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe o Canal
   if ((canal = medidor->ExisteCanal(eqpto, canalP, fases)) == NULL)
      {//cria um novo Canal
      canal = medidor->InsereCanal(eqpto, canalP, fases);
      }
   canal->Id              = FalsoId();
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoChave::InsereCanal_Q(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe o Canal
   if ((canal = medidor->ExisteCanal(eqpto, canalQ, fases)) == NULL)
      {//cria um novo Canal
      canal  = medidor->InsereCanal(eqpto, canalQ, fases);
      }
   canal->Id              = FalsoId();
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoChave::InsereCanal_V(VTMedidor *medidor, AnsiString codigo, VTEqpto *eqpto, int fases, double tolerancia_perc)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe o Canal
   if ((canal = medidor->ExisteCanal(eqpto, canalV, fases)) == NULL)
      {//cria um novo Canal
      canal = medidor->InsereCanal(eqpto, canalV, fases);
      }
   canal->Id              = FalsoId();
   canal->Codigo          = codigo;
   canal->Tolerancia_perc = tolerancia_perc;
   canal->Enabled         = true;
   InsereCurva(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoChave::InsereCurva(VTCanal *canal)
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
bool __fastcall TMontaMedicaoChave::RetiraMedicao(void)
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
   //atualiza o gráfico
   grafico->Redraw();

   return(true);
   }

//---------------------------------------------------------------------------
//eof

