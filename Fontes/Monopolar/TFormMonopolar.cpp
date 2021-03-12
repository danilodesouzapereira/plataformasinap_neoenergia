//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMonopolar.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\TEdtCurto.h"
#include "..\Constante\Fases.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Especial.h"
#include "..\..\DLL_Inc\Fasometro.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Medidor.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMonopolar::TFormMonopolar(TForm *form_owner, VTApl *apl_owner, TWinControl *parent, int ind_pat)
                          : TFormEdt(form_owner, parent)
   {
   //salva ponteiros p/ objetos
   this->apl = apl_owner;
   //salva patamar p/ exibir gráfico de fluxo
   this->ind_pat = ind_pat;
   //cria objetos locais
//   flow   = DLL_NewObjFlow(apl);
   montaZ = DLL_NewObjMontaZ(apl);
   lisEQP = new TList();
   //inicia CBoxRede
   CBoxRedeInicia();
   //inicia CBoxTrecho
   CBoxTrechoInicia();
   //inicia CBoxFase
   CBoxFaseInicia(faseABCN);
   //configura objeto Edita para notificar alterações na Rede
   ConfiguraObjEdita(true);
   //salva opções do fluxo
   SalvaCfgFluxo();
   }

//---------------------------------------------------------------------------
__fastcall TFormMonopolar::~TFormMonopolar(void)
   {
   //variáveis locais
//   VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //restaura opções do fluxo e marca que não está atualizado
   RestauraCfgFluxo();
   //configura objeto Edita para não notificar alterações na Rede
   ConfiguraObjEdita(false);
   //restaura Trecho com defeito
   RestauraTrechoComDefeito();
   //apgaga eventual Moldura
   if (grafico) grafico->Moldura();
   //destrói objetos locais
   if (montaZ) {delete montaZ; montaZ = NULL;}
   if (edt)    {delete edt;    edt    = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //importante: marca que o objeto Flow existente no objeto Apl não está atualizado
//   flow->UpToDate = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ActionHelpExecute(TObject *Sender)
	{
	//variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Abertura_de_Fase");
	if(formHelp) formHelp->Show();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ActionRedeAlteradaExecute(TObject *Sender)
   {
   //verifica se há um trecho selecionado
   if (defeito.trecho != NULL)
      {//define o mesmo Trecho com defeito
      DefineTrechoComDefeito(defeito.trecho);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ActionEdtExecute(TObject *Sender)
   {
   try{//se necessário, cria um objeto TEdtBusca
      if (edt != NULL) delete edt;
      //cria TEdtCurto p/ controle do mouse
      edt = DLL_NewObjEdtCurto(apl, ActionMouseOff, ActionMouseOn, ActionSelEqptoClickSimples);
      ((TEdtCurto*)edt)->DefineActionCurto(ActionSelEqptoClickDuplo);
      //atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ActionSelEqptoClickDuploExecute(TObject *Sender)
   {
   //variáveis locais
   VTTrecho  *trecho;
   VTEqpto  *eqpto = (VTEqpto*)Sender;
   TList     *lisEXT = (TList*)Sender;

   //proteção: verifica se já foi selecionado um eqpto
   if (eqpto == NULL) return;
   //verifica se o Eqpto é um Trecho
   if (eqpto->Tipo() != eqptoTRECHO) return;
   trecho = (VTTrecho*)eqpto;
   //define Trecho com defeito
   DefineTrechoComDefeito(trecho);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ActionSelEqptoClickSimplesExecute(TObject *Sender)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::AlteraMatZ(VTMatZ *matZ, int ind_fase, double comp_km)
   {
   //variáveis locais
   complex<double> z_min;

   // Valor abaixo alterado em 2016.06.04 (HPS)
   // double          r_infinito = 1e10/comp_km;
   double          r_infinito = 1.e8 / comp_km;

   //proteção
   if (ind_fase >= matZ->Dimensao) return;
   //define impedância "infinita" p/ o elemento [ind_fase][ind_fase]
   matZ->Z[ind_fase][ind_fase] = complex<double>(r_infinito, 0);
   //define impedância nula p/ linha e coluna
   for (int i = 0; i < matZ->Dimensao; i++)
      {
      if (i == ind_fase) continue;
      matZ->Z[ind_fase][i] = CZero();
      matZ->Z[i][ind_fase] = CZero();
      }
   //acerta a impedância do neutro
   if (indNEUTRO >= matZ->Dimensao) return;
   if (IsDoubleZero(Abs(matZ->Z[indNEUTRO][indNEUTRO])))
      {//determina menor impedância na diagonal
      z_min = matZ->Z[indFASE_A][indFASE_A];
      for (int i = indFASE_B; i <= indFASE_C; i++)
         {
         if (Abs(matZ->Z[i][i]) < Abs(z_min)) z_min = matZ->Z[i][i];
         }
      //atualiza impedância de neutro
      matZ->Z[indNEUTRO][indNEUTRO] = z_min;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::AnalisaDefeito(VTTrecho *trecho, int ind_fase)
   {
   //variáveis locais
   VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTMercado *mercado  = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //verifica se é um defeito novo
   if ((defeito.trecho == trecho)&&(defeito.ind_fase == ind_fase)) return;
   //restaura eventual Trecho selecionado anteriormente
   RestauraTrechoComDefeito();
   //define Trecho com defeito
   ModificaTrechoComDefeito(trecho, ind_fase);
   //configura Flow p/ utilizar Carga como Zcte incorporada na Ynodal
   flow->UpToDate    = false;
   flow->ModeloCarga = cargaZCTE;
   flow->TipoFluxo = fluxoGAUSS3;
   //recalcula fluxo de potência
   flow->CalculaRedeUnica(redes);
   //atualiza grafico
   //if (ind_pat < 0) grafico->Redraw();
   //else             grafico->Show(grafFLOW, ano_carga, ind_pat);
   //ind_pat = -1;
   grafico->Show(grafFLOW, mercado->AnoAtual, ind_pat);
   //atualiza eventuais janelas abertas
   DLL_AtualizaFormMedidor(apl->Form);
   DLL_AtualizaFormFasometro(apl->Form);
   DLL_Especial_Atualiza(apl->Form);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxFaseChange(TObject *Sender)
   {
   //variáveis locais
   VTTrecho  *trecho;

   //restaura eventual Trecho selecionado anteriormente
   RestauraTrechoComDefeito();
   //verifica se existe um Trecho selecionado
   if (CBoxTrecho->ItemIndex < 0) return;
   if ((trecho = (VTTrecho*)CBoxTrecho->Items->Objects[CBoxTrecho->ItemIndex]) == NULL) return;
   //verifica se existe uma fase selecionada
   if (CBoxFase->ItemIndex < 0) return;
   //analisa defeito p/ fase do Trecho já selecionado
   AnalisaDefeito(trecho, CBoxFase->ItemIndex);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxFaseInicia(int fases_tag)
   {
   //variáveis locais
   int      index;
   int      fase[] = {faseA, faseB, faseC, faseN};
   VTFases *fases  = (VTFases*)apl->GetObject(__classid(VTFases));

   //desabilita evento OnChange
   CBoxFase->OnChange = NULL;
   //salva fase atualmente selecionada
   index  = CBoxFase->ItemIndex;
   //reinicia CBoxFase
   CBoxFase->Clear();
   for (int n = 0; n < sizeof(fase)/sizeof(int); n++)
      {//verifica se o Arranjo possui a fase
      if (fases->Fase1ContemFase2(fases_tag, fase[n]))
         {//inclui fase no CBoxFase
         CBoxFase->Items->Add("Fase " + fases->AsStrUpper[fase[n]]);
         }
      }
   //pré-seleciona uma fase, de preferência a que estava anteriormente selecionada
   if ((index >= 0)&&(index < CBoxFase->Items->Count))          CBoxFase->ItemIndex = index;
   if ((CBoxFase->Items->Count > 0)&&(CBoxFase->ItemIndex < 0)) CBoxFase->ItemIndex = 0;
   //reabilita evento OnChange
   CBoxFase->OnChange = CBoxFaseChange;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxRedeChange(TObject *Sender)
   {
   //restaura eventual Trecho selecionado anteriormente
   RestauraTrechoComDefeito();
   //reinicia CBoxTrecho
   CBoxTrechoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxRedeInicia(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE  = redes->LisRede();

   //reinicia CBoxRede
   CBoxRede->Clear();
   CBoxRede->Sorted = false;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //ordena CBoxRede
   CBoxRede->Sorted = true;
   //NÃO pré-seleciona nenhuma rede
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxTrechoChange(TObject *Sender)
   {
   //variáveis locais
   VTTrecho  *trecho;

   //verifica se existe um Trecho selecionado
   if (CBoxTrecho->ItemIndex < 0) return;
   //determina Trecho selecionado
   if ((trecho = (VTTrecho*)CBoxTrecho->Items->Objects[CBoxTrecho->ItemIndex]) == NULL) return;
   //redefine Trecho com defeito
   DefineTrechoComDefeito(trecho);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::CBoxTrechoInicia(void)
   {
   //variáveis locais
   VTRede   *rede;
   VTTrecho *trecho;

   //reinicia CBoxTrecho
   CBoxTrecho->Clear();
   CBoxTrecho->Sorted = false;
   //verifica se existe uma Rede selecionada em CBoxRede
   if (CBoxRede->ItemIndex < 0) return;
   if ((rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex]) == NULL) return;
   //obtém lista de Trechos da Rede
   lisEQP->Clear();
   rede->LisLigacao(lisEQP, eqptoTRECHO);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      CBoxTrecho->Items->AddObject(trecho->Codigo, trecho);
      }
   //ordena CBoxRede
   CBoxTrecho->Sorted = true;
   //não pré-seleciona nenhum Trecho
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ConfiguraObjEdita(bool insere_action)
   {
   //variáveis locais
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   if (insere_action) {edita->ActionOnChangeRedeInsere(ActionRedeAlterada);}
   else               {edita->ActionOnChangeRedeRetira(ActionRedeAlterada);}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::DefineTrechoComDefeito(VTTrecho *trecho)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //restaura eventual Trecho selecionado anteriormente
   RestauraTrechoComDefeito();
   //seleciona Rede em CBoxRede
   CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(trecho->rede);
   //reinicia CBoxTrecho e seleciona o Trecho
   CBoxTrechoInicia();
   CBoxTrecho->ItemIndex = CBoxTrecho->Items->IndexOfObject(trecho);
   //reinicia CBoxFase
   CBoxFaseInicia(trecho->arranjo->Fases);
   //verifica se existe uma fase selecionada
   if (CBoxFase->ItemIndex < 0) return;
   //exibe moldura no Trecho
   grafico->Moldura(trecho);
   //analisa abertura de fase
   AnalisaDefeito(trecho, CBoxFase->ItemIndex);
   /*
   //verifica se existe uma fase selecionada
   if (CBoxFase->ItemIndex < 0) return;
   //salva Trecho do defeito e executa análise da rede
   AnalisaDefeito(trecho, CBoxFase->ItemIndex);
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::ModificaTrechoComDefeito(VTTrecho *trecho, int ind_fase)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //salva dados do defeito
   defeito.trecho           = trecho;
   defeito.ind_fase         = ind_fase;
   defeito.arranjo_original = trecho->arranjo;
   //cria um novo Arranjo igual ao Arranjo original
   arranjo = defeito.arranjo_original->Clone();
   arranjo->Owner = NULL;
   //NÃO altera as fases do Arranjo
   //arranjo->Fases = fases->Fase1ExclusaoFase2(arranjo->Fases, fase);
   //calcula MatZ do Arranjo
   montaZ->Executa(arranjo);
   //altera tipo do Arranjo DEPOIS de calcular a MatZ
   arranjo->TipoCfg = arranjoMAT_Z;
   //acerta matriz MatZ, de acordo c/ o defeito
   AlteraMatZ(arranjo->MatZ, ind_fase, trecho->Comprimento_km);
   //cancela Arranjo atual do Trecho (p/ evitar que ele seja destruído)
   trecho->arranjo = NULL;
   //redefine Arranjo do Trecho
   trecho->DefineObjArranjo(arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //variáveis locais
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //esconde o Form
   Action = caHide;
   //restaura Trecho com defeito
   RestauraTrechoComDefeito();
   //exibe gráfico de topologia (que destrói este Form)
   sinap_child->ActionTopo->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::FormShow(TObject *Sender)
   {
   //posiciona Form
   Top  = 0;
   Left = 0;
   //inicia objeto EdtExplorer
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::RestauraCfgFluxo(void)
   {
   //variáveis locais
   VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));

   flow->ModeloCarga = modelo_carga;
   flow->TipoFluxo = tipo_fluxo;
   //marca flow desatualizado
   flow->UpToDate = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::RestauraTrechoComDefeito(void)
   {
   //variáveis locais
   //VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se há um Trecho selecionado
   if (defeito.trecho != NULL)      //OOPS: não deveria destruir arranjo clonado?
      {//redefine Arranjo do Trecho
      defeito.trecho->DefineObjArranjo(defeito.arranjo_original);
      //cancela ponteiros
      defeito.trecho           = NULL;
      defeito.arranjo_original = NULL;
      //exibe gráfico de topologia
      //grafico->Show(grafTOPO);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMonopolar::SalvaCfgFluxo(void)
   {
   //variáveis locais
   VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));

   modelo_carga = flow->ModeloCarga;
   tipo_fluxo = flow->TipoFluxo;
   }

//---------------------------------------------------------------------------
//eof


