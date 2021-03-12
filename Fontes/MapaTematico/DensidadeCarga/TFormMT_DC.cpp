//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormMT_DC.h"
#include "TFormRel_DC.h"
#include "VTTema_DC.h"
#include "..\..\Apl\TApl.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Grafico\VTDraw.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
//#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Redegraf\VTCelula.h"
#include "..\..\Redegraf\VTCelulas.h"
#include "..\..\Redegraf\VTRedegraf.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Radial.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMT_DC::TFormMT_DC(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose)
   : TForm(Owner)
   {
   //vari�veis locais
   VTMercado  *mercado  = (VTMercado*)apl_owner->GetObject(__classid(VTMercado));
   VTRedegraf *redegraf = (VTRedegraf*)apl_owner->GetObject(__classid(VTRedegraf));

   //salva ActionOnClose
   this->ActionOnClose = ActionOnClose;
   //salva ano do mercado
   original.ano_mercado = mercado->AnoAtual;
   //salva modo gr�fico
   original.modo_grafico = redegraf->ModoGrafico;
   //insere Form em seu Parent
   if (parent) Parent = parent;
   //cria objeto Apl pr�prio
   apl = NewObjApl(this, apl_owner);
   //cria demais objetos
   apl->Add(tema_DC = NewObjTema_DC(apl));
   lisEQP    = new TList();
   lisCLASSE = new TList();
   /*
   //verifica op��o de borda
   if (! borda)
      {//muda BorderStyle
      BorderStyle = bsNone;
      //bloqueia op��o de fechar o Forms
      ActionFecha->Visible = false;
      //desabilita op��o exibir/esconder redes
      GBoxRedeVisible->Visible     = false;
      //CheckBoxRedeVisible->OnClick = NULL;
      ExibeRedes(false);
      }
   */
   //verifica se foi definida ActionOnClose
   if (ActionOnClose != NULL)
      {//desabilita op��o exibir/esconder redes
      GBoxRedeVisible->Visible     = false;
      //CheckBoxRedeVisible->OnClick = NULL;
      ExibeRedes(false);
      }
   //configura gr�fico em modo geo-referenciado
   SelecionaModoGrafico(modoGEOREFERENCIADO);
   //inicia CBoxArea, CLBoxClasse e LViewRede
   CBoxAreaInicia();
   CLBoxClasseInicia();
   LViewRedeInicia();
   //inicia CBoxAno e CBoxPatamar
   CBoxAnoInicia();
   CBoxPatamarInicia();
   //simula sele��o de resolu��o p/ atualizar o gr�fico
   //Atualiza();
   //CBoxAreaChange(NULL);
   CalculaDensidadeCargaDefault(); //fkm 15.07.2016
   }

//---------------------------------------------------------------------------
__fastcall TFormMT_DC::~TFormMT_DC(void)
   {
   //vari�veis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //desabilita Timer
   if (Timer->Enabled) ActionAnimacaoExecute(NULL);
   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //salva redes habilitadas
   SalvaCBoxRede();
   //torna a exibir as redes
   if (! CheckBoxRedeVisible->Checked) ExibeRedes(true);
   //desabilita mapa tem�tico
   HabilitaMapaTematico(false);
   //restaura modo gr�fico
   SelecionaModoGrafico(original.modo_grafico);
   //fecha FormRel_DC
   if (FormRel_DC) {delete FormRel_DC; FormRel_DC = NULL;}
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   if (lisCLASSE) {delete lisCLASSE; lisCLASSE = NULL;}
   //restaura ano do mercado
   mercado->AnoAtual = original.ano_mercado;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionAnimacaoExecute(TObject *Sender)
   {
   //atualiza lista de patamares habilitados
//   CBoxPatamarInicia();       //DVK 2013.07.17
   //vari�veis locais
   //VTStk *stk = (VTStk*)apl->GetObject(__classid(VTStk));

   //habilita/desabilita Editor
   //stk->ActionEditor->Enabled = Timer->Enabled;
   //habilita/desabilita Timer
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption
   ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionCalculoExecute(TObject *Sender)
   {
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionHelpExecute(TObject *Sender)
   {
   //variaveis locais
   TForm *formHelp;

   formHelp = DLL_NewFormHelp(this, apl, "Mapa_Tem�tico_Densidade_de_Carga");
   if(formHelp) formHelp->Show();

   }
//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionRelatorioExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         timer_enabled;

   //verifica se j� existe FormRel_DC
   if (FormRel_DC == NULL)
      {//cria FormRel_DC
      FormRel_DC = new TFormRel_DC(this, apl, Parent, ActionRemontaMapaPadrao);
      FormRel_DC->Atualiza(RadioGroupGrafico->ItemIndex != 1);
      }
   //exibe
   FormRel_DC->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionRemontaMapaDegradeExecute(TObject *Sender)
   {
   //verifica se o Mapa degrade est� sendo exibido
   if (RadioGroupGrafico->ItemIndex == 1)
      {
      tema_DC->MontaGraficoDegrade(PanelColor->Color);
      //habilita mapa tem�tico
      HabilitaMapaTematico(true);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionRemontaMapaPadraoExecute(TObject *Sender)
   {
   //verifica se o Mapa padr�o est� sendo exibido
   if (RadioGroupGrafico->ItemIndex == 0)
      {//refaz os c�lculos e monta o gr�fico indicado
      RecalculaDensidadeCarga();
      tema_DC->MontaGraficoPadrao();
      //habilita mapa tem�tico
      HabilitaMapaTematico(true);
      }
   //verifica se o Mapa demanda m�xima est� sendo exibido
   if (RadioGroupGrafico->ItemIndex == 2)
      {//refaz os c�lculos e monta o gr�fico indicado
      RecalculaDensidadeCarga();
      tema_DC->MontaGraficoDemandaMax();
      //habilita mapa tem�tico
      HabilitaMapaTematico(true);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionRemontaPorDemandaMaxExecute(TObject *Sender)
   {
   //verifica se o Mapa demanda m�xima est� sendo exibido
   if (RadioGroupGrafico->ItemIndex == 2)
      {//refaz os c�lculos e monta o gr�fico indicado
      RecalculaDensidadeCarga();
      tema_DC->MontaGraficoDemandaMax();
      //habilita mapa tem�tico
      HabilitaMapaTematico(true);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionSelAllExecute(TObject *Sender)
   {
   //vari�veis locais
   TListItem *Item;
   VTRede    *rede;

   //seleciona todos items em CLBoxCCS
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      Item = LViewRede->Items->Item[n];
      rede = (VTRede*)Item->Data;
      Item->Checked            = true;
      rede->Status[sttENABLED] = true;
      }
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionSelAllClasseExecute(TObject *Sender)
   {
   //vari�veis locais
   TListItem *Item;

   //desabilita action temporariamente
   ActionCalculo->Enabled = false;
   //seleciona todos items em CLBoxClasse
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      CLBoxClasse->Checked[n] = true;
      }
   //reabilita action
   ActionCalculo->Enabled = true;
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionSelNoneExecute(TObject *Sender)
   {
   //vari�veis locais
   TListItem *Item;;
   VTRede    *rede;

   //seleciona todos items em CLBoxCCS
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      Item = LViewRede->Items->Item[n];
      rede = (VTRede*)Item->Data;
      Item->Checked            = false;
      rede->Status[sttENABLED] = false;
      }
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ActionSelNoneClasseExecute(TObject *Sender)
   {
   //vari�veis locais
   TListItem *Item;

   //desabilita action temporariamente
   ActionCalculo->Enabled = false;
   //seleciona todos items em CLBoxClasse
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      CLBoxClasse->Checked[n] = false;
      }
   //reabilita action
   ActionCalculo->Enabled = true;
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::Atualiza(void)
   {
   //vari�veis locais
   //VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //calcula densidade de carga
   //RecalculaDensidadeCarga();  //fkm  2016.07.15
   //atualiza mapa tem�tico
   RadioGroupGraficoClick(NULL);
   //atualiza FormRel_DC
   if (FormRel_DC != NULL) FormRel_DC->Atualiza(RadioGroupGrafico->ItemIndex != 1);

   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CalculaDensidadeCargaDefault(void)
   {
   //vari�veis locais
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
   int ind_pat;
   //vari�veis locais
   int        dim_celula_m;
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //determina resolu��o (dimens�o da c�lula)
   dim_celula_m = StrToDouble(CBoxArea->Text);
   //redefine Grid de c�lulas
   celulas->DefineGridQuadrado(dim_celula_m);
   //faz os c�lculos e monta o gr�fico indicado
   //redefine ano do Mercado
   mercado->AnoAtual = mercado->AnoInicial + CBoxAno->ItemIndex;
   //monta lista de Redes selecionadas
   lisEQP->Clear();
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      if (LViewRede->Items->Item[n]->Checked)
         {
         lisEQP->Add(LViewRede->Items->Item[n]->Data);
         }
      }
   //monta lista de Redes selecionadas
   lisCLASSE->Clear();
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      if (CLBoxClasse->Checked[n])
         {
         lisCLASSE->Add(CLBoxClasse->Items->Objects[n]);
         }
      }
   //recalcula densidade de carga das celulas
   ind_pat = IndicePatamarSelecionado();
   tema_DC->Executa(lisEQP, lisCLASSE, ind_pat);
   tema_DC->FaixaValoresUniforme();
   tema_DC->MontaFaixaDemanda();
   tema_DC->MontaGraficoPadrao();
   //habilita mapa tem�tico
   HabilitaMapaTematico(true);
   }
//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CarregaCBoxRede(void)
   {
   //variaveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //VTRede *rede;
   TListItem *Item;
   int habilitada=1; //0=falso; 1=verdadeiro

   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      Item = LViewRede->Items->Item[n];
      //rede = (VTRede*)Item->Data;
      //obtem redes e status
      if (geral->Infoset->GetInfo(Item->SubItems->Strings[0], habilitada))
         {
         if(habilitada == 1)      Item->Checked = true;
         else                    Item->Checked = false;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CBoxAnoInicia(void)
   {
   //vari�veis locais
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //reinicia CBoxPatamar c/ Patamares
   CBoxAno->Clear();
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pr�-seleciona primeiro ano
   if (CBoxAno->Items->Count > 0) CBoxAno->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CBoxAreaChange(TObject *Sender)
   {
   //vari�veis locais
   int        dim_celula_m;
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //determina resolu��o (dimens�o da c�lula)
   dim_celula_m = StrToDouble(CBoxArea->Text);
   //redefine Grid de c�lulas
   celulas->DefineGridQuadrado(dim_celula_m);
   //recalcula densidade de carga e atualiza o gr�fico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CBoxAreaInicia(void)
   {
   //vari�veis locais
   int        dim_max, dim_min, exp, delta;
   int        dim_ref[] = {100, 200, 250, 300, 400, 500, 1000, 2000, 5000};
   VTArea     *area;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //determina �rea da rede
   area = redegraf->Area(redes);
   //determina dimens�o da celula p/ que resulte um grid com 50x50 celulas
   dim_max = (max(area->Width(), area->Height())) / 50;
   //determina dimens�o m�nima da c�lula p/ que resulte em um grid de 80x80
   dim_min = (max(area->Width(), area->Height())) / 80;
   //converte dimens�es p/ metros
   if ((dim_max *= 0.01) <= 0) dim_max = 1;
   if ((dim_min *= 0.01) <= 0) dim_min = 1;
   //arredonda dim_max p/ valor m�ltiplo de 10
   exp = Log10(double(dim_max));
   dim_max = RoundTo(double(dim_max), exp);
   //arredonda dim_min p/ valor m�ltiplo de 10
   exp = Log10(double(dim_min));
   dim_min = RoundTo(double(dim_min), exp);
   if (dim_max <= dim_min) dim_max++;
   //determina incremento
   delta = dim_max - dim_min;
   if (delta < 10)
      {delta  = 1;}
   else
      {
      exp   = Log10(double(delta));
      delta = RoundTo(double(delta), exp) / 5;
      }
   //preenche CBoxArea
   CBoxArea->Clear();
   //preenche dimens�es obrigat�rias no in�cio
   for (int n = 0; n < sizeof(dim_ref)/sizeof(int); n++)
      {
      if (dim_min > dim_ref[n])
         {//insere op��o obrigat�ria de quadr�cula
         CBoxArea->Items->Add(DoubleToStr("%5.1f",  dim_ref[n]));
         }
      }
   //preenche com as op��es entre dim_min e dim_max
   for (int dim = dim_min; dim <= (dim_max + delta); dim += delta)
      {
      CBoxArea->Items->Add(DoubleToStr("%5.1f", dim));
      }
   //preenche dimens�es obrigat�rias no final
   for (int n = 0; n < sizeof(dim_ref)/sizeof(int); n++)
      {
      if (dim_max < dim_ref[n])
         {//insere op��o obrigat�ria de quadr�cula
         CBoxArea->Items->Add(DoubleToStr("%5.1f",  dim_ref[n]));
         }
      }
   //pr� seleciona valor intermedi�rio
   CBoxArea->ItemIndex = (CBoxArea->Items->Count-1) / 2;
   }

//---------------------------------------------------------------------
void __fastcall TFormMT_DC::CBoxPatamarInicia(void)
   {
   //vari�veis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //TList       *lisPAT    = patamares->LisPatamar();
   TList *lisPAT  = new TList();

   //guarda patamares habilitados
   patamares->LisPatamarHabilitado(lisPAT);

   //reinicia CBoxPatamar c/ Patamares
   CBoxPatamar->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pr�-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   delete lisPAT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CLBoxClasseInicia(void)
   {
   //vari�veis locais
   int       index;
   VTClasse  *classe;
   VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
   TList     *lisCLASSE = classes->LisClasse();

   //reinicia CLBoxClasse
   CLBoxClasse->Clear();
   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      index  = CLBoxClasse->Items->AddObject(classe->Codigo, classe);
      CLBoxClasse->Checked[index] = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::CheckBoxRedeVisibleClick(TObject *Sender)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //exibe/esconde as redes
   ExibeRedes(CheckBoxRedeVisible->Checked);
   //atualiza Grafico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::ExibeRedes(bool visible)
   {
   //vari�veis locais
   VTRede    *rede;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //exibe/esconde as redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede  = (VTRede*)lisREDE->Items[n];
      rede->Show[eqp_VISIBLE] = visible;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //executa ActionOnClose
   if (ActionOnClose) ActionOnClose->Execute();
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona no canto superior esquerdo
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::HabilitaMapaTematico(bool enabled)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresenta��o do mapa tem�tico
   grafico->MapaTematicoEnabled = enabled;
   }

//---------------------------------------------------------------------------
int __fastcall TFormMT_DC::IndicePatamarSelecionado(void)
   {
   //vari�veis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
   patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
   }
//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.Column == Column)
      {//inverte ordem
      LViewSort.OrdemCrescente  = ! LViewSort.OrdemCrescente;
      }
   else
      {//salva coluna usada p/ ordena��o
      LViewSort.Column         = Column;
      LViewSort.OrdemCrescente = true;
      }
   //for�a reordena��o
   ((TListView*)Sender)->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::LViewCompare(TObject *Sender,
                TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //vari�veis locais
   int       index;
   double    valor1, valor2;

   //prote��o
   if (LViewSort.Column == NULL) return;
   //determina index p/ substrings
   index = LViewSort.Column->Index - 1;
   //verifica se coluna de Caption
   if (LViewSort.Column->Index == 0)
      {//coluna de Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   else if (LViewSort.Column->Alignment == taLeftJustify)
      {//coluna com texto
      Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
      }
   else
      {//coluna com valor num�rico
      valor1  = StrToDouble(Item1->SubItems->Strings[index]);
      valor2  = StrToDouble(Item2->SubItems->Strings[index]);
      if      (valor1 < valor2) Compare = -1;
      else if (valor1 > valor2) Compare =  1;
      else                      Compare =  0;
      }
   //verifica se ordem crescente/decrescente
   if (! LViewSort.OrdemCrescente) Compare = - Compare;
   }
//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::LViewRedeClick(TObject *Sender)
   {
   //vari�veis locais
   int       num_alteracao = 0;
   TListItem *Item;
   VTRede    *rede;

   //verifica se foi alterada a op��o de incluir/excluir uma Rede
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      Item = LViewRede->Items->Item[n];
      rede = (VTRede*)Item->Data;
      if (Item->Checked != rede->Status[sttENABLED])
         {//atualiza estado da rede
         rede->Status[sttENABLED] = Item->Checked;
         num_alteracao++;
         }
      }
      //recalcula densidade de carga e atualiza mapa tem�tico
   if (num_alteracao > 0) Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::LViewRedeInicia(void)
   {
   //vari�veis locais
   int        index;
   TListItem *Item;
   VTRede    *rede;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //reinicia LViewRede
   LViewRede->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede                     = (VTRede*)lisREDE->Items[n];
      rede->Status[sttENABLED] = true;
      Item                     = LViewRede->Items->Add();
      Item->Data               = rede;
      Item->Caption            = rede->TipoRede->Codigo;
      Item->Checked            = rede->Status[sttENABLED];
      Item->SubItems->Add(rede->Codigo);
      }
   if (geral->Infoset->LoadFromFile("redes_habilitadas")) CarregaCBoxRede();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::PanelColorClick(TObject *Sender)
   {
   //exibe ColorDialog p/ usu�rio selecionar a cor
   ColorDialog->Color = PanelColor->Color;
   if (ColorDialog->Execute())
      {//verifica se houve troca de cor
      if (ColorDialog->Color != PanelColor->Color)
         {
         PanelColor->Color = ColorDialog->Color;
         //atualiza as cores do mapa tem�tico degrade
         ActionRemontaMapaDegrade->Execute();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::RadioGroupGraficoClick(TObject *Sender)
   {

   if (RadioGroupGrafico->ItemIndex == 0)
      {//refaz c�lculos e monta o gr�fico
      ActionRemontaMapaPadrao->Execute();
      //atualiza FormRel_DC p/ exibir/esconder Checkboxes
      if (FormRel_DC) FormRel_DC->Atualiza(RadioGroupGrafico->ItemIndex == 0);
      }
   if (RadioGroupGrafico->ItemIndex == 1)
      {//apenas monta o gr�fico indicado, n�o � necess�rio refazer os c�lculos
      ActionRemontaMapaDegrade->Execute();
      }
   if (RadioGroupGrafico->ItemIndex == 2)
      {//refaz c�lculos e monta o gr�fico
      ActionRemontaPorDemandaMax->Execute();
      //atualiza FormRel_DC p/ exibir/esconder Checkboxes
      if (FormRel_DC) FormRel_DC->Atualiza(RadioGroupGrafico->ItemIndex == 2);
      }

   //exibe/esconde par�metros de patamares e anos
   //GroupBox2->Visible = !(RadioGroupGrafico->ItemIndex == 2);
   Panel2->Visible = !(RadioGroupGrafico->ItemIndex == 2);
   GroupBox4->Visible = !(RadioGroupGrafico->ItemIndex == 2);
   //exibe/esconde PanelColor
   PanelColor->Visible = (RadioGroupGrafico->ItemIndex == 1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::RecalculaDensidadeCarga(void)
   {
   //vari�veis locais
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
   int ind_pat;

   //redefine ano do Mercado
   mercado->AnoAtual = mercado->AnoInicial + CBoxAno->ItemIndex;
   //monta lista de Redes selecionadas
   lisEQP->Clear();
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      if (LViewRede->Items->Item[n]->Checked)
         {
         lisEQP->Add(LViewRede->Items->Item[n]->Data);
         }
      }
   //monta lista de Redes selecionadas
   lisCLASSE->Clear();
   for (int n = 0; n < CLBoxClasse->Items->Count; n++)
      {
      if (CLBoxClasse->Checked[n])
         {
         lisCLASSE->Add(CLBoxClasse->Items->Objects[n]);
         }
      }
   //recalcula densidade de carga das celulas
   //tema_DC->Executa(lisEQP, lisCLASSE, CBoxPatamar->ItemIndex);
   if (RadioGroupGrafico->ItemIndex == 2)
      {
      tema_DC->Executa(lisEQP, lisCLASSE);
      }
   else
      {
      ind_pat = IndicePatamarSelecionado();
      tema_DC->Executa(lisEQP, lisCLASSE, ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::SalvaCBoxRede(void)
   {
   //variaveis locais
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTRede     *rede;
   TListItem *Item;
   AnsiString nome_rede;
   int        habilitada;

   //limpa Infoset
   geral->Infoset->Clear();
   //loop p/ todas as redes
   for (int n = 0; n < LViewRede->Items->Count; n++)
      {
      Item = LViewRede->Items->Item[n];
      rede = (VTRede*)Item->Data;
       //guarda nome da rede e status
      if (Item->Checked  == true)  habilitada = 1;
      else habilitada = 0;
      geral->Infoset->AddInfo(rede->Codigo, habilitada);
      }
   geral->Infoset->SaveToFile("redes_habilitadas");
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::SelecionaModoGrafico(int modo)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //verifica se est� sendo alterado o modo gr�fico
   if (redegraf->ModoGrafico != modo)
      {//seleciono modo gr�fico indicado
      redegraf->ModoGrafico = modo;
      grafico->ModoGraficoAlterado();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_DC::TimerTimer(TObject *Sender)
   {
   //verifica se est� habilitada altera��o do patamar
   if (CheckBoxPatamar->Checked)
      {//seleciona novo patamar
      if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
         {//seleciona pr�ximo patamar
         CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
         }
      else
         {//seleciona primeiro patamar
         CBoxPatamar->ItemIndex = 0;
         //verifica se est� habilitada altera��o do ano
         if (CheckBoxAno->Checked)
            {//seleciona novo ano
            if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
               {//seleciona pr�ximo ano
               CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
               }
            else
               {
               CBoxAno->ItemIndex = 0;
               }
            }
         }
      }
   //verifica se est� habilitada a altera��o do ano
   else if (CheckBoxAno->Checked)
      {//seleciona novo ano
      if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
         {//seleciona pr�ximo ano
         CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
         }
      else
         {
         CBoxAno->ItemIndex = 0;
         }
      }
   //recalcula densidade de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
//eof

void __fastcall TFormMT_DC::CheckBoxTransparenteClick(TObject *Sender)
   {
   //implementar...?

   }
//---------------------------------------------------------------------------




