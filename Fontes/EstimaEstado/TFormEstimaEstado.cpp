//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEstimaEstado.h"
#include "TFormCfgEstimaEstado.h"
#include "VTEstimaEstado.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\VTEditor.h"
#include "..\Estimador\VTEstimador.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRedes.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Equivalente.h"
#include "..\..\DLL_Inc\Funcao.h"
//TESTES
#include "..\Flow\VTFlow.h"
#include "TMontaMedicao.h"
#include "TMontaMedicaoRedeLivro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEstimaEstado::TFormEstimaEstado(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {
   //cria objeto Apl local
   apl = NewObjApl(this, apl_owner);
   apl->Add(estima = NewObjEstimaEstado(apl));
   apl->Add(editor    = DLL_NewObjEditor(apl));
   //cria demais objetos
   lisEQP = new TList();
   //inser Form em seu Prent
   if (parent) Parent = parent;
   //inicia controle da ordenação dos LView
   LViewSort.column     = NULL;
   LViewSort.ascendente = true;
   //exibe opções selecionadas para a estimação
   ExibeOpcoes();
   //torna todos os Medidores visíveis
   ExibeMedidores(true);
   //exibe Medidores presentes na rede
   LViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormEstimaEstado::~TFormEstimaEstado(void)
   {
   //variáveis locais
   TForm *form;

   //fecha FormEquivalente
   while ((form = ExisteForm("TFormEquivalente", this)) != NULL) {delete form;}
   //torna todos os Medidores invisíveis
   ExibeMedidores(false);
   //destrói objeto Apl local
   if (apl) {delete apl; apl = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionCfgParametroExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *Form;

   try{//cria TFormCfgEstimaEstado como janela modal
      Form = new TFormCfgEstimaEstado(this, apl, NULL);
      if (Form->ShowModal() == mrOk)
         {//atualiza tela
         ExibeOpcoes();
         }
      //destrói TFormCfgEstimaEstado
      delete Form;
      }catch(Exception &e)
         {
         Erro("Erro na configuração de opções para estimação de estados");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEstimaEstadoExecute(TObject *Sender)
   {
   //executa EstimaEstado
   if (estima->Executa())
      {
      Aviso("Estimação de estado executada corretamente");
      }
   else
      {
      Erro("Estimação de estado não foi executada corretamente");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEdtExecute(TObject *Sender)
   {
   //editor de alterações
   editor->EditaAltera(NULL, NULL, ActionRedeChanged);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEdtMedicaoExecute(TObject *Sender)
   {
   //editor de Medições
   editor->EditaMedicao(NULL, NULL, ActionRedeChanged);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionFechaExecute(TObject *Sender)
   {
   //fecha o Form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionRedeChangedExecute(TObject *Sender)
   {
   //atauliza LView
   LViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionRedutorExecute(TObject *Sender)
   {
   //variáveis locais
   TForm *form;

   form = DLL_NewFormEqv(this, apl, Parent);
   form->Show();
   //delete form;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ExibeMedidores(bool visible)
   {
   //variáveis locais
   VTMedidor *medidor;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Medidores da rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   //loop p/ todos Medidores
   for (int i = 0; i < lisEQP->Count; i++)
      {
      medidor = (VTMedidor*)lisEQP->Items[i];
      medidor->Show[eqp_VISIBLE] = visible;
      }
   //atualiza o gráfico
   if (lisEQP->Count > 0) grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ExibeOpcoes(void)
   {
   //variáveis locais
   AnsiString txt;

   //reinicia RichEdit
   RichEdit->Lines->Clear();

   // Patamar
   txt = "Patamar:                                         " + AnsiString(estima->Patamar);
   RichEdit->Lines->Add(txt);

   //tipo de estimação
   txt = "Tipo de rede:                                    ";
   if (estima->Tipo == estimaDESEQUILIBRADO) txt += "Desequilibrada";
   else                                      txt += "Equilibrada";
   RichEdit->Lines->Add(txt);

   // Tipo de referência
   txt = "Tipo de referência:                              ";
   if     (estima->TipoBarraRef == brFIXOS_1THETA_0V) txt += "Barra única - ângulo da tensão fixo";
   else if(estima->TipoBarraRef == brFIXOS_1THETA_1V) txt += "Barra única - ângulo e módulo da tensão fixos";
   else if(estima->TipoBarraRef == brFIXOS_nTHETA_0V) txt += "Múltiplas barras - ângulos de tensão fixos";
   else                                               txt += "Múltiplas barras - ângulos e módulos de tensão fixos";
   RichEdit->Lines->Add(txt);

   // VDNM
   txt = "Utiliza VDNM                                     ";
   if(estima->UtilizaVDNM) txt += "Sim";
   else                    txt += "Não";
   RichEdit->Lines->Add(txt);
   if(estima->UtilizaVDNM)
      {
      txt  = "   Número máximo de montagens de [H]:            ";
      txt += AnsiString(estima->NumMaxMontH);
      RichEdit->Lines->Add(txt);
      }

   // Relatório de fluxo de potência
   txt = "Imprime relatório de fluxo de potência:          ";
   if (estima->RelFluxoPotencia) txt += "Sim";
   else                          txt += "Não";
   RichEdit->Lines->Add(txt);

   // Método de cálculo
   txt = "Método de Estimação de Estado:                   ";
   if     (estima->MetodoEE == meeCONVENCIONAL)   txt += "Convencional";
   else if(estima->MetodoEE == meeQR_FAST_GIVENS) txt += "Decomposição QR - Fast Givens";
   else if(estima->MetodoEE == meeQR_GIVENS)      txt += "Decomposição QR - Givens";
   else if(estima->MetodoEE == meeQR_HOUSEHOLDER) txt += "Decomposição QR - Householder";
   else                                           txt += "*****";
   RichEdit->Lines->Add(txt);

   // Ordenação de linhas (QR)
   // Proteção: ordenação de linhas implementada somente no método QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   txt = "Critério para ordenação de linhas (QR):          ";
   if     (estima->OrdenacaoLinhasQR == qrlSEM_ORDENACAO)     txt += "Sem ordenação";
   else if(estima->OrdenacaoLinhasQR == qrlMENOR_NUM_ELEM_FD) txt += "Menor número de elementos FD";
   else                                                       txt += "Menor última coluna";
   RichEdit->Lines->Add(txt);

   //particionamento da análise
   txt = "Particionamento da análise:                      ";
   switch(estima->TipoGrupo)
      {
      case grupoUNICO:        txt += "Estimação única para a rede completa";                    break;
      case grupoSUBESTACAO:   txt += "Estimação por subestação e seus primários e secundários"; break;
      case grupoPRIMARIO:     txt += "Estimação por primário e seus secundários";               break;
      case grupoINDEPENDENTE: txt += "Estimação por grupos de redes independentes";             break;
      default:                txt += "Indefinido";                                              break;
      }
   RichEdit->Lines->Add(txt);
   //perda no ferro
   txt = "Perda no ferro de transformadores:               ";
   if (estima->PerdaFerroEnabled) txt += "Habilitada";
   else                           txt += "Desabilitada";
   RichEdit->Lines->Add(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.column == Column)
      {//inverte ordem
      LViewSort.ascendente = ! LViewSort.ascendente;
      }
   else
      {//redefine coluna usada p/ ordenação
      LViewSort.column     = Column;
      LViewSort.ascendente = true;
      }
   //força reordenação
   LView->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare)
   {
   //variáveis locais
   int index = LViewSort.column->Index - 1;

   //verifica Coluna sendo ordenada
   if (LViewSort.column->Index == 0)
      {//compara Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   else
      {//comparação de colunas com string
      Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
      }
   //verifica se ordem ascendente/descendente
   if (! LViewSort.ascendente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::LViewInicia(void)
   {
   //variáveis locais
   TList     *lisCANAL;
   TListItem *Item;
   VTCanal   *canal;
   VTMedidor *medidor;
   VTFases   *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia LView
   LView->Clear();
   //obtém lista de Medidores da rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   //loop p/ todos Medidores
   for (int i = 0; i < lisEQP->Count; i++)
      {
      medidor = (VTMedidor*)lisEQP->Items[i];
      //loop p/ todos Canais do Medidor
      lisCANAL = medidor->LisCanal();
      for (int j = 0; j < lisCANAL->Count; j++)
         {
         canal = (VTCanal*)lisCANAL->Items[j];
         //insere novo ListItem
         Item          = LView->Items->Add();
         Item->Data    = canal;
         Item->Caption = medidor->Codigo;
         Item->SubItems->Add(canal->Codigo);
         Item->SubItems->Add(canal->TipoCanalAsString);
         Item->SubItems->Add(canal->Eqpto->TipoAsString());
         Item->SubItems->Add(canal->Eqpto->Codigo);
         Item->SubItems->Add(fases->AsStrABCN[canal->Fases]);
         }
      }

   // Atualiza "Caption" de GroupBox2
   GroupBox2->Caption = AnsiString("Medidores instalados (total ") +
                        AnsiString(LView->Items->Count)            +
                        AnsiString(")");
   }

//---------------------------------------------------------------------------
//eof

//TESTES
//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ButTesteClick(TObject *Sender)
   {
   //variáveis locais
   TMontaMedicaoRedeLivro *monta;

   monta = new TMontaMedicaoRedeLivro(apl);
   monta->Executa(estima->Tipo == estimaDESEQUILIBRADO);
   delete monta;
   //atualiza tela
   LViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ButRetAUGClick(TObject *Sender)
   {
   //variáveis locais
   TMontaMedicao *monta;
   VTFlow        *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //se necessário, executa o flxo
   if (! flow->UpToDate) flow->CalculaPorRede(redes);
   monta = new TMontaMedicao(apl);
   monta->Executa(estima->Tipo == estimaDESEQUILIBRADO);
   delete monta;
   //atualiza tela
   LViewInicia();
   }

//---------------------------------------------------------------------------


