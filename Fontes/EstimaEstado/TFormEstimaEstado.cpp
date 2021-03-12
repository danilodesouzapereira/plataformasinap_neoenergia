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
   //inicia controle da ordena��o dos LView
   LViewSort.column     = NULL;
   LViewSort.ascendente = true;
   //exibe op��es selecionadas para a estima��o
   ExibeOpcoes();
   //torna todos os Medidores vis�veis
   ExibeMedidores(true);
   //exibe Medidores presentes na rede
   LViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormEstimaEstado::~TFormEstimaEstado(void)
   {
   //vari�veis locais
   TForm *form;

   //fecha FormEquivalente
   while ((form = ExisteForm("TFormEquivalente", this)) != NULL) {delete form;}
   //torna todos os Medidores invis�veis
   ExibeMedidores(false);
   //destr�i objeto Apl local
   if (apl) {delete apl; apl = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionCfgParametroExecute(TObject *Sender)
   {
   //vari�veis locais
   TForm *Form;

   try{//cria TFormCfgEstimaEstado como janela modal
      Form = new TFormCfgEstimaEstado(this, apl, NULL);
      if (Form->ShowModal() == mrOk)
         {//atualiza tela
         ExibeOpcoes();
         }
      //destr�i TFormCfgEstimaEstado
      delete Form;
      }catch(Exception &e)
         {
         Erro("Erro na configura��o de op��es para estima��o de estados");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEstimaEstadoExecute(TObject *Sender)
   {
   //executa EstimaEstado
   if (estima->Executa())
      {
      Aviso("Estima��o de estado executada corretamente");
      }
   else
      {
      Erro("Estima��o de estado n�o foi executada corretamente");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEdtExecute(TObject *Sender)
   {
   //editor de altera��es
   editor->EditaAltera(NULL, NULL, ActionRedeChanged);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ActionEdtMedicaoExecute(TObject *Sender)
   {
   //editor de Medi��es
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
   //vari�veis locais
   TForm *form;

   form = DLL_NewFormEqv(this, apl, Parent);
   form->Show();
   //delete form;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ExibeMedidores(bool visible)
   {
   //vari�veis locais
   VTMedidor *medidor;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Medidores da rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   //loop p/ todos Medidores
   for (int i = 0; i < lisEQP->Count; i++)
      {
      medidor = (VTMedidor*)lisEQP->Items[i];
      medidor->Show[eqp_VISIBLE] = visible;
      }
   //atualiza o gr�fico
   if (lisEQP->Count > 0) grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::ExibeOpcoes(void)
   {
   //vari�veis locais
   AnsiString txt;

   //reinicia RichEdit
   RichEdit->Lines->Clear();

   // Patamar
   txt = "Patamar:                                         " + AnsiString(estima->Patamar);
   RichEdit->Lines->Add(txt);

   //tipo de estima��o
   txt = "Tipo de rede:                                    ";
   if (estima->Tipo == estimaDESEQUILIBRADO) txt += "Desequilibrada";
   else                                      txt += "Equilibrada";
   RichEdit->Lines->Add(txt);

   // Tipo de refer�ncia
   txt = "Tipo de refer�ncia:                              ";
   if     (estima->TipoBarraRef == brFIXOS_1THETA_0V) txt += "Barra �nica - �ngulo da tens�o fixo";
   else if(estima->TipoBarraRef == brFIXOS_1THETA_1V) txt += "Barra �nica - �ngulo e m�dulo da tens�o fixos";
   else if(estima->TipoBarraRef == brFIXOS_nTHETA_0V) txt += "M�ltiplas barras - �ngulos de tens�o fixos";
   else                                               txt += "M�ltiplas barras - �ngulos e m�dulos de tens�o fixos";
   RichEdit->Lines->Add(txt);

   // VDNM
   txt = "Utiliza VDNM                                     ";
   if(estima->UtilizaVDNM) txt += "Sim";
   else                    txt += "N�o";
   RichEdit->Lines->Add(txt);
   if(estima->UtilizaVDNM)
      {
      txt  = "   N�mero m�ximo de montagens de [H]:            ";
      txt += AnsiString(estima->NumMaxMontH);
      RichEdit->Lines->Add(txt);
      }

   // Relat�rio de fluxo de pot�ncia
   txt = "Imprime relat�rio de fluxo de pot�ncia:          ";
   if (estima->RelFluxoPotencia) txt += "Sim";
   else                          txt += "N�o";
   RichEdit->Lines->Add(txt);

   // M�todo de c�lculo
   txt = "M�todo de Estima��o de Estado:                   ";
   if     (estima->MetodoEE == meeCONVENCIONAL)   txt += "Convencional";
   else if(estima->MetodoEE == meeQR_FAST_GIVENS) txt += "Decomposi��o QR - Fast Givens";
   else if(estima->MetodoEE == meeQR_GIVENS)      txt += "Decomposi��o QR - Givens";
   else if(estima->MetodoEE == meeQR_HOUSEHOLDER) txt += "Decomposi��o QR - Householder";
   else                                           txt += "*****";
   RichEdit->Lines->Add(txt);

   // Ordena��o de linhas (QR)
   // Prote��o: ordena��o de linhas implementada somente no m�todo QR_GIVENS
   if(estima->MetodoEE != meeQR_GIVENS) estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   txt = "Crit�rio para ordena��o de linhas (QR):          ";
   if     (estima->OrdenacaoLinhasQR == qrlSEM_ORDENACAO)     txt += "Sem ordena��o";
   else if(estima->OrdenacaoLinhasQR == qrlMENOR_NUM_ELEM_FD) txt += "Menor n�mero de elementos FD";
   else                                                       txt += "Menor �ltima coluna";
   RichEdit->Lines->Add(txt);

   //particionamento da an�lise
   txt = "Particionamento da an�lise:                      ";
   switch(estima->TipoGrupo)
      {
      case grupoUNICO:        txt += "Estima��o �nica para a rede completa";                    break;
      case grupoSUBESTACAO:   txt += "Estima��o por subesta��o e seus prim�rios e secund�rios"; break;
      case grupoPRIMARIO:     txt += "Estima��o por prim�rio e seus secund�rios";               break;
      case grupoINDEPENDENTE: txt += "Estima��o por grupos de redes independentes";             break;
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
   //destr�i o Form
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
      {//redefine coluna usada p/ ordena��o
      LViewSort.column     = Column;
      LViewSort.ascendente = true;
      }
   //for�a reordena��o
   LView->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare)
   {
   //vari�veis locais
   int index = LViewSort.column->Index - 1;

   //verifica Coluna sendo ordenada
   if (LViewSort.column->Index == 0)
      {//compara Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   else
      {//compara��o de colunas com string
      Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
      }
   //verifica se ordem ascendente/descendente
   if (! LViewSort.ascendente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstimaEstado::LViewInicia(void)
   {
   //vari�veis locais
   TList     *lisCANAL;
   TListItem *Item;
   VTCanal   *canal;
   VTMedidor *medidor;
   VTFases   *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia LView
   LView->Clear();
   //obt�m lista de Medidores da rede
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
   //vari�veis locais
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
   //vari�veis locais
   TMontaMedicao *monta;
   VTFlow        *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //se necess�rio, executa o flxo
   if (! flow->UpToDate) flow->CalculaPorRede(redes);
   monta = new TMontaMedicao(apl);
   monta->Executa(estima->Tipo == estimaDESEQUILIBRADO);
   delete monta;
   //atualiza tela
   LViewInicia();
   }

//---------------------------------------------------------------------------


