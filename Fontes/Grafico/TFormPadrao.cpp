// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormPadrao.h"
#include "TPadrao.h"
#include "VTGrafico.h"
#include "VTGrid.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\VTEdt.h"
#include "..\Figura\VTFiguras.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\TAtributo.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Legenda.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormPadrao::TFormPadrao(TForm* Owner, VTApl *apl, TWinControl *parent)
	: TFormEdt(Owner, parent)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva ponteiros
	my_owner = Owner;
	this->apl = apl;
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grid = geral->grid;
	// cria objetos
	legenda = DLL_NewObjLegenda(apl);
	lisATR = new TList();
	lisEQP = new TList();
	// inicia cboxGridDim
	CBoxGridDimInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormPadrao::~TFormPadrao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// apaga moldura
	Moldura();
	// destr�i objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	if (legenda)
	{
		delete legenda;
		legenda = NULL;
	}
	// destr�i lista e seus objetos
	if (lisATR)
	{
		LimpaTList(lisATR);
		delete lisATR;
		lisATR = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAlteraCorExecute(TObject *Sender)
{ // DVK 2014.12.02 alterado para ActionMudaCorExecute
	/*
	 //determina origem do evento
	 TPanel *panel = (TPanel*)Sender;

	 //passa o foco p/ o Form
	 my_owner->SetFocus();
	 //exibe dialog de sele��o de cor
	 ColorDialog->Color = panel->Color;
	 if (ColorDialog->Execute())
	 {//determina nova cor
	 panel->Color = ColorDialog->Color;
	 //l� nova configura��o
	 LeConfiguracao();
	 //atualiza o gr�fico
	 grafico->Redraw();
	 } */
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAlteraPadraoExecute(TObject *Sender)
{
	// l� nova configura��o
	LeConfiguracao();
	// atualiza o gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAtributoAllEqptoExecute(TObject *Sender)
{
	AtualizaAtributoAllEqpto(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAtributoChangedExecute(TObject *Sender)
{
	// vari�veis locais
	TAtributo *atributo;

	// determina Atributo alterador
	if (clboxEqpto->ItemIndex < 0)
		return;
	atributo = (TAtributo*)clboxEqpto->Items->Objects[clboxEqpto->ItemIndex];
	atributo->enabled = clboxEqpto->Checked[clboxEqpto->ItemIndex];
	// atualiza atributo do Eqpto
	AtualizaAtributo(atributo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAtributoSelAllExecute(TObject *Sender)
{
	// vari�veis locais
	TAtributo *atributo;

	// cancelsa sele��o de todos os items do CLBoxEqpto
	for (int n = 0; n < clboxEqpto->Items->Count; n++)
	{
		atributo = (TAtributo*)clboxEqpto->Items->Objects[n];
		atributo->enabled = true;
		clboxEqpto->Checked[n] = true;
	}
	// for�a atualiza��o dos atributos do Eqpto e atualiza o gr�fico
	AtualizaAtributo(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionAtributoSelNoneExecute(TObject *Sender)
{
	// vari�veis locais
	TAtributo *atributo;

	// cancelsa sele��o de todos os items do CLBoxEqpto
	for (int n = 0; n < clboxEqpto->Items->Count; n++)
	{
		atributo = (TAtributo*)clboxEqpto->Items->Objects[n];
		atributo->enabled = false;
		clboxEqpto->Checked[n] = false;
	}
	// for�a atualiza��o dos atributos do Eqpto e atualiza o gr�fico
	AtualizaAtributo(NULL);
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::ActionComentarioBordaCorExecute(TObject *Sender)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //define cor inicial do ColorDialog
   if (lisCOM->Count > 0)
      {
      comentario = (VTFiguraComentario*)lisCOM->First();
      ColorDialog->Color = comentario->Retangulo->BordaCor;
      }
   //exibe ColorDialog
   if (ColorDialog->Execute())
      {//redefine a cor de todos os comentarios
      for (int n = 0; n < lisCOM->Count; n++)
         {
         comentario                      = (VTFiguraComentario*)lisCOM->Items[n];
         comentario->Retangulo->BordaCor = ColorDialog->Color;
         }
      //atualiza o gr�fico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::ActionComentarioBordaEspessuraExecute(TObject *Sender)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //redefine estilo da borda todos os comentarios
   for (int n = 0; n < lisCOM->Count; n++)
      {
      comentario  = (VTFiguraComentario*)lisCOM->Items[n];
      comentario->Retangulo->BordaEspessura = CBoxBordaEspessura->ItemIndex + 1;
      //qualquer espessura diferente de 1 exige borda psSolid
      if (comentario->Retangulo->BordaEspessura > 1)
         {
         comentario->Retangulo->BordaEstilo = psSolid;
         }
      }
   //atualiza o gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::ActionComentarioBordaEstiloExecute(TObject *Sender)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //redefine estilo da borda todos os comentarios
   for (int n = 0; n < lisCOM->Count; n++)
      {
      comentario = (VTFiguraComentario*)lisCOM->Items[n];
      switch(CBoxBordaEstilo->ItemIndex)
         {
         case 0:  comentario->Retangulo->BordaEstilo = psSolid;      break;
         case 1:  comentario->Retangulo->BordaEstilo = psDash;       break;
         case 2:  comentario->Retangulo->BordaEstilo = psDot;        break;
         case 3:  comentario->Retangulo->BordaEstilo = psDashDot;    break;
         case 4:  comentario->Retangulo->BordaEstilo = psDashDotDot; break;
         case 5:  comentario->Retangulo->BordaEstilo = psClear;      break;
         default: comentario->Retangulo->BordaEstilo = psSolid;      break;
         }
      //qualquer estilo diferente de psSolid exige espessura 1
      if (comentario->Retangulo->BordaEstilo != psSolid)
         {
         comentario->Retangulo->BordaEspessura = 1;
         }
      }
   //atualiza o gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::ActionComentarioFundoColorExecute(TObject *Sender)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //define cor inicial do ColorDialog
   if (lisCOM->Count > 0)
      {
      comentario = (VTFiguraComentario*)lisCOM->First();
      ColorDialog->Color = comentario->Retangulo->FundoCor;
      }
   //exibe ColorDialog
   if (ColorDialog->Execute())
      {//redefine a cor de todos os comentarios
      for (int n = 0; n < lisCOM->Count; n++)
         {
         comentario                        = (VTFiguraComentario*)lisCOM->Items[n];
         comentario->Retangulo->FundoCor = ColorDialog->Color;
         }
      //atualiza o gr�fico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::ActionComentarioFontExecute(TObject *Sender)
   {
   //vari�veis locais
   int           dx, dy, height, width;
   AnsiString    txt;
   TStrings     *texto;
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //define Font inicial do FontDialog
   if (lisCOM->Count > 0)
      {
      comentario = (VTFiguraComentario*)lisCOM->First();
      FontDialog->Font->Assign(comentario->Font);
      }
   //exibe FontDialog
   if (FontDialog->Execute())
      {//redefine a Font de todos os comentarios
      for (int n = 0; n < lisCOM->Count; n++)
         {
         comentario = (VTFiguraComentario*)lisCOM->Items[n];
         texto      = comentario->Texto();
         //atualiza Font
         comentario->Font->Assign(FontDialog->Font);
         //redefine dimens�es da moldura em fun��o das dimens�es do texto
         dx = dy = 0;
         ImageFont->Canvas->Font->Assign(comentario->Font);
         for (int n = 0; n < texto->Count; n++)
            {
            txt    = texto->Strings[n];
            width  = abs(ImageFont->Canvas->TextWidth(txt))+1;
            height = abs(ImageFont->Canvas->TextHeight(txt)) * texto->Count;
            //grafico->SizeOfString(txt, width, height);
            if (height > dy) dy = height;
            if (width  > dx) dx = width;
            }
         comentario->Retangulo->SetDimensao(dx + 12, dy + 6);
         }
      //atualiza o gr�fico
      grafico->Redraw();
      }
   }

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necess�rio, cria um objeto TEdtExplorer
		if (edt != NULL)
			delete edt;
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
		// simula alter��o do TreeView p/ atualizar moldura
		tviewChange(tview, tview->Selected);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Gr�fico");
	if (formHelp)
		formHelp->Show();

}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionMudaCorExecute(TObject *Sender)
{ // DVK 2014.12.02 antigo ActionAlteraCorExecute
	// determina origem do evento
	TPanel *panel = (TPanel*)Sender;

	// passa o foco p/ o Form
	my_owner->SetFocus();
	// exibe dialog de sele��o de cor
	ColorDialog->Color = panel->Color;
	if (ColorDialog->Execute())
	{ // determina nova cor
		panel->Color = ColorDialog->Color;
		// l� nova configura��o
		LeConfiguracao();
		// atualiza o gr�fico
		grafico->Redraw();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelEqptoExecute(TObject *Sender)
{
	// vari�veis locais
	VTEqpto *eqpto = NULL;
	TTreeNode *node_eqpto = NULL;
	TList *lisTMP = (TList*)Sender;

	// prote��o
	if (Sender == NULL)
		return;
	if (lisTMP->Count > 100)
	{ // aviso //DVK 2014.12.10
		switch (Confirma("H� mais de 100 eqptos selecionados.",
			"Tem certeza que deseja selecionar todos eles?", MB_YESNOCANCEL))
		{
		case IDYES:
			break;
		case IDNO:
			return;
		case IDCANCEL:
			return;
		default:
			return;
		}
	}
	// loop p/ todos Eqptos recebidos em lisTMP (Sender)
	for (int n = 0; n < lisTMP->Count; n++)
	{
		if ((eqpto = (VTEqpto*)lisTMP->Items[n]) == NULL)
			continue;
		// verifica se o Eqpto n�o havia sido selecionado anteriormente
		if (!TreeViewExisteEqpto(eqpto))
		{ // atualiza lista de Eqptos selecionados
			lisEQP->Add(eqpto);
			// atualiza TreeView
			node_eqpto = TreeViewInsereEqpto(eqpto);
		}
	}
	// verifica se n�o foi inserido nenhum TreeNode mas havia Eqptos selecionados em lisTMP
	if ((node_eqpto == NULL) && (eqpto != NULL))
	{ // determina TreeNode do Eqptos
		node_eqpto = TreeViewExisteEqpto(eqpto);
	}
	// verifica se foi inclu�do um Eqpto
	if (node_eqpto != NULL)
	{ // expande somente Node do Eqpto inclu�do
		if (!node_eqpto->Parent->Expanded)
		{
			tview->FullCollapse();
			node_eqpto->Parent->Expand(false);
		}
		// pr�-seleciona Node do Eqpto
		tview->Selected = node_eqpto;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelGridAlignExecute(TObject *Sender)
{
	// salva alinhamento do grid
	grid->align = cboxGridAlign->ItemIndex;
	// atualiza gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelGridCorExecute(TObject *Sender)
{
	// exibe ColorDialog
	ColorDialog->Color = panelGridCor->Color;
	if (ColorDialog->Execute())
	{ // seleciona nova cor de fundo
		grid->cor = ColorDialog->Color;
		panelGridCor->Color = grid->cor;
		// atualiza gr�fico
		grafico->Redraw();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelGridDimExecute(TObject *Sender)
{
	// prote��o
	if (cboxGridDim->ItemIndex < 0)
		return;
	// salva dimens�o do grid
	CalculaDimensaoGrid();
	// atualiza gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelGridVisibleExecute(TObject *Sender)
{
	// salva op��o de exibir/esconder grid
	grid->visible = checkGridExibir->Checked;
	// atualiza o gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionSelLegendaCorExecute(TObject *Sender)
{
	// exibe ColorDialog
	ColorDialog->Color = panelLegendaCor->Color;
	if (ColorDialog->Execute())
	{ // seleciona nova cor de legenda
		grafico->Padrao->legenda.cor = ColorDialog->Color;
		panelLegendaCor->Color = ColorDialog->Color;
		// atualiza gr�fico
		grafico->Redraw();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormPadrao::ActionTViewReiniciaExecute(TObject *Sender)
{
	// reinicia TreeView
	TreeViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::AtualizaAtributo(TAtributo *atributo)
{
	// vari�veis locais
	TTreeNode *raiz;

	// descarta TreeNodes n�o associados c/ Eqptos
	if (tview->Selected == NULL)
		return;
	// determina TreeNode raiz
	for (raiz = tview->Selected; raiz->Level > 0; raiz = raiz->Parent);
	// verifica o TreeNode raiz
	if (raiz == node_raiz.geral)
		return; // nada a fazer
	if (raiz == node_raiz.reducao)
	{ // Eqpto � uma Reducao
		if (tview->Selected->Level == 0)
			AtualizaAtributoAllEqpto(atributo);
		else if (tview->Selected->Level == 1)
			AtualizaAtributoEqpto(atributo);
	}
	else if (raiz == node_raiz.eqpto)
	{ // demais Eqptos
		if (tview->Selected->Level == 1)
			AtualizaAtributoAllEqpto(atributo);
		else if (tview->Selected->Level == 2)
			AtualizaAtributoEqpto(atributo);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::AtualizaAtributoAllEqpto(TAtributo *atributo)
{
	// vari�veis locais
	VTEqpto *eqpto;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se deve atualizar um �nico Atributo ou todos os Atributos
	if (atributo != NULL)
	{ // atualiza um �nico Atributo: determina Eqpto
		eqpto = atributo->eqpto;
		// obt�m lista de todos Eqptos do mesmo tipo
		lisEQP->Clear();
		if (eqpto->Tipo() == eqptoREDUCAO)
		{
			redes->LisReducaoCarregada(lisEQP);
		}
		else
		{
			redes->LisEqpto(lisEQP, eqpto->Tipo());
		}
		redes->LisEqpto(lisEQP, eqpto->Tipo());
		for (int ne = 0; ne < lisEQP->Count; ne++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[ne];
			eqpto->Show[atributo->bit_index] = atributo->enabled;
			// verifica se o Eqpto � uma Reducao
			if (eqpto->Tipo() == eqptoREDUCAO)
				AtualizaAtributoEqptoReducao((VTReducao*)eqpto);
		}
	}
	else
	{ // atualiza todos Atributos: determina Eqpto
		if (clboxEqpto->Items->Count == 0)
			return;
		atributo = (TAtributo*)clboxEqpto->Items->Objects[0];
		eqpto = atributo->eqpto;
		// obt�m lista de todos Eqptos do mesmo tipo
		lisEQP->Clear();
		if (eqpto->Tipo() == eqptoREDUCAO)
		{
			redes->LisReducaoCarregada(lisEQP);
		}
		else
		{
			redes->LisEqpto(lisEQP, eqpto->Tipo());
		}
		redes->LisEqpto(lisEQP, eqpto->Tipo());
		for (int ne = 0; ne < lisEQP->Count; ne++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[ne];
			// aplica a configura��o de atributos a todos Eqptos do mesmo tipo
			for (int n = 0; n < clboxEqpto->Items->Count; n++)
			{
				atributo = (TAtributo*)clboxEqpto->Items->Objects[n];
				eqpto->Show[atributo->bit_index] = clboxEqpto->Checked[n];
			}
			// verifica se o Eqpto � uma Reducao
			if (eqpto->Tipo() == eqptoREDUCAO)
				AtualizaAtributoEqptoReducao((VTReducao*)eqpto);
		}
	}
	// atualiza gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::AtualizaAtributoEqpto(TAtributo *atributo)
{
	// vari�veis locais
	VTEqpto *eqpto;

	// atualiza atributos do Eqpto
	for (int n = 0; n < clboxEqpto->Items->Count; n++)
	{
		atributo = (TAtributo*)clboxEqpto->Items->Objects[n];
		eqpto = atributo->eqpto;
		eqpto->Show[atributo->bit_index] = clboxEqpto->Checked[n];
	}
	// verifica se o Eqpto � uma Reducao
	if (eqpto->Tipo() == eqptoREDUCAO)
		AtualizaAtributoEqptoReducao((VTReducao*)eqpto);
	// atualiza gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::AtualizaAtributoEqptoReducao(VTReducao* reducao)
{
	// vari�veis locais
	VTEqpto *eqpto;
	TList *lisLIG = reducao->LisLigacaoEsquematico();

	// atualiza atributos dos Trechos fict�cios de acordo c/ os atributos da Reducao
	for (int n = 0; n < lisLIG->Count; n++)
	{
		eqpto = (VTEqpto*)lisLIG->Items[n];
		eqpto->ShowAsUnsigned = 0;
		eqpto->Show[eqp_VISIBLE] = reducao->Show[eqp_VISIBLE];
		eqpto->Show[eqp_FIXO] = reducao->Show[eqp_FIXO];
		eqpto->Show[eqp_ID] = reducao->Show[eqp_ID];
		eqpto->Show[eqp_CODIGO] = reducao->Show[eqp_CODIGO];
		eqpto->Show[lig_P] = reducao->Show[rdu_P];
		eqpto->Show[lig_Q] = reducao->Show[rdu_Q];
		eqpto->Show[lig_S] = reducao->Show[rdu_S];
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormPadrao::CalculaDimensaoGrid(void)
 {
 //vari�veis locais
 int      num_quad;
 VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTArea  *area = redes->AreaEsquematico();

 //determina n�mero de quadr�culas selecionado pelo usu�rio
 num_quad  = cboxGridDim->Text.ToInt();
 grid->dim = area->Width() / num_quad;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::CalculaDimensaoGrid(void)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTArea *area = grafico->AreaTotal;

	// define dimens�o da quadr�cula
	grid->escala = cboxGridDim->Text.ToInt();
	grid->dim = grid->escala * (area->Width() / 600.);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormPadrao::CBoxGridDimInicia(void)
 {
 //reinicia cboxGridDim
 cboxGridDim->Clear();
 //configura op��es de 500 a 200 p/ grid
 for (int n = 500; n >= 200; n -= 100)
 {
 cboxGridDim->Items->Add(IntToStr(n));
 }
 //configura op��es de 100 a 10 p/ grid
 for (int n = 100; n >= 10; n -= 10)
 {
 cboxGridDim->Items->Add(IntToStr(n));
 }
 //pr�-seleciona a primeira op��o
 cboxGridDim->ItemIndex = 0;
 //salva dimens�o do grid
 CalculaDimensaoGrid();
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::CBoxGridDimInicia(void)
{
	// reinicia cboxGridDim
	cboxGridDim->Clear();
	// configura op��es de 1 a 10
	for (int n = 1; n <= 10; n++)
	{
		cboxGridDim->Items->Add(IntToStr(n));
	}
	// n�o pr�-seleciona nenhum item
	cboxGridDim->ItemIndex = -1;
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::CheckBoxComentarioVisibleClick(TObject *Sender)
{
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //exibe esconde todos comentarios
   for (int n = 0; n < lisCOM->Count; n++)
	  {
	  comentario = (VTFiguraComentario*)lisCOM->Items[n];
	  comentario->Visible = CheckBoxComentarioVisible->Checked;
	  }
   //atualiza o gr�fico
   grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::EqptoShowAsUnsigned(int eqpto_tipo, unsigned &checked,
	unsigned &grayed)
{
	// vari�veis locais
	VTEqpto *eqpto;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// inicia vazlores
	checked = 0x0;
	grayed = 0x0;
	// obt�m lista de Eqptos
	lisEQP->Clear();
	if (eqpto_tipo == eqptoREDUCAO)
	{
		redes->LisReducaoCarregada(lisEQP);
	}
	else
	{
		redes->LisEqpto(lisEQP, eqpto_tipo);
	}
	if (lisEQP->Count == 0)
		return;
	// inicia valores c/ dados do primeiro
	eqpto = (VTEqpto*)lisEQP->First();
	checked = eqpto->ShowAsUnsigned;
	grayed = eqpto->ShowAsUnsigned;
	for (int n = 1; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		checked &= eqpto->ShowAsUnsigned;
		grayed |= eqpto->ShowAsUnsigned;
	}
	grayed = checked ^ grayed;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destr�i o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
	// inicia tela c/ a configura��o definida
	IniciaGeral();
	TreeViewInicia();
	// cria editor
	ActionEdtExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::IniciaCBox(TComboBox *cbox, AnsiString opcao)
{
	// procura item do CBox igual � op��o indicada
	for (int n = 0; n < cbox->Items->Count; n++)
	{
		if (cbox->Items->Strings[n] == opcao)
		{
			cbox->ItemIndex = n;
			return;
		}
	}
	// n�o encontrou op��o: seleciona a primeira
	cbox->ItemIndex = 0;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormPadrao::IniciaCLBoxEqpto(VTEqpto *eqpto, bool all_eqpto)
 {
 //vari�veis locais
 TAtributo *atributo;

 //desabilita ActionAtributoChange
 ActionAtributoChanged->Enabled = false;
 //reinicia
 clboxEqpto->Clear();
 //prote��o
 if (eqpto != NULL)
 {//configura GroupBox, ToolBarEqpto e CLBox
 gboxEqpto->Caption    = eqpto->TipoAsString() + ": " + ((all_eqpto) ? AnsiString("*.*") : eqpto->Codigo);
 butCfgPadrao->Visible = ! all_eqpto;
 clboxEqpto->Color = (all_eqpto) ? clSkyBlue : clWindow;
 //obt�m lista de Atributos do Eqpto
 legenda->LisAtributos(eqpto, lisATR);
 //apreenche CLBox c/ atributos
 for (int n = 0; n < lisATR->Count; n++)
 {
 atributo = (TAtributo*)lisATR->Items[n];
 //clboxEqpto->Items->AddObject(atributo->nome, atributo);
 clboxEqpto->Items->AddObject(atributo->nome + " " + atributo->unidade, atributo);
 if (all_eqpto)
 {//atributos v�lidos p/ todos os Eqptos do mesmo tipo
 clboxEqpto->Checked[n] = (atributo->bit_index == eqp_VISIBLE);
 }
 else
 {//atributos v�lidos somente p/ o Eqpto
 clboxEqpto->Checked[n] = eqpto->Show[atributo->bit_index];
 }
 }
 }
 //reabilita ActionAtributoChange
 ActionAtributoChanged->Enabled = true;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::IniciaCLBoxEqpto(VTEqpto *eqpto, bool all_eqpto)
{
	// vari�veis locais
	unsigned checked, grayed;
	TAtributo *atributo;

	// desabilita ActionAtributoChange
	ActionAtributoChanged->Enabled = false;
	// reinicia
	clboxEqpto->Clear();
	// prote��o
	if (eqpto != NULL)
	{ // configura GroupBox, ToolBarEqpto e CLBox
		gboxEqpto->Caption = eqpto->TipoAsString() + ": " + ((all_eqpto) ? AnsiString("*.*") :
			eqpto->Codigo);
		butCfgPadrao->Visible = !all_eqpto;
		clboxEqpto->Color = (all_eqpto) ? clWhite : clBtnFace;
		// obt�m lista de Atributos do Eqpto
		legenda->LisAtributos(eqpto, lisATR);
		// determina palavra de atributos habilitados
		if (all_eqpto)
		{
			EqptoShowAsUnsigned(eqpto->Tipo(), checked, grayed);
		}
		else
		{
			// EqptoShowAsUnsigned(eqpto, checked, grayed);
			checked = eqpto->ShowAsUnsigned;
			grayed = 0x0;
		}
		// apreenche CLBox c/ atributos
		for (int n = 0; n < lisATR->Count; n++)
		{
			atributo = (TAtributo*)lisATR->Items[n];
			// clboxEqpto->Items->AddObject(atributo->nome, atributo);
			clboxEqpto->Items->AddObject(atributo->nome + " " + atributo->unidade, atributo);
			clboxEqpto->Checked[n] = ((checked & atributo->bit_index) != 0);
			if ((grayed & atributo->bit_index) != 0)
				clboxEqpto->State[n] = cbGrayed;
		}
	}
	// reabilita ActionAtributoChange
	ActionAtributoChanged->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::IniciaGBoxGrid(void)
{
	// inicia GBox c/ configura��o do Grid
	checkGridExibir->Checked = grid->Visible();
	cboxGridDim->ItemIndex = cboxGridDim->Items->IndexOf(IntToStr(grid->escala));
	if (cboxGridDim->ItemIndex < 0)
		cboxGridDim->ItemIndex = 0;
	cboxGridAlign->ItemIndex = grid->align;
	if (cboxGridAlign->ItemIndex < 0)
		cboxGridAlign->ItemIndex = 0;
	panelGridCor->Color = grid->cor;
	// salva dimens�o do grid
	CalculaDimensaoGrid();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::IniciaGeral(void)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// desabilita Actions
	ActionAlteraPadrao->Enabled = false;
	// inicia cores de fundo
	panelFundoCor->Color = redegraf->CorFundo;
	panelBordaCor->Color = (panelFundoCor->Color == clBtnFace) ? clBlack : clBtnFace;
	// inicia cor de eqpto. isolado
	panelIsoladoCor->Color = grafico->Padrao->isolado.cor;
	// inicia configura��es de barra
	panelBarCor->Color = grafico->Padrao->barra.cor;
	IniciaCBox(cboxBarDim, IntToStr(grafico->Padrao->barra.dim));
	// inicia configura��es de trafo //DVK 2014.12.09
	IniciaCBox(cboxTrfDim, IntToStr(grafico->Padrao->trafo.dim));
	// inicia configura��es de ligacoes
	IniciaCBox(cboxLigDim, IntToStr(grafico->Padrao->ligacao.dim));
	// inicia configura��es de chave
	IniciaCBox(cboxChvDim, IntToStr(grafico->Padrao->chave.dim));
	RadioGroupChvCor->ItemIndex = grafico->Padrao->chave.op_cor;
	// inicia cor da legenda
	panelLegendaCor->Color = grafico->Padrao->legenda.cor;
	RadioGroupLegenda->ItemIndex = (grafico->Padrao->legenda.cor_rede ? 0 : 1);
	RadioGroupLegendaClick(NULL);
	// inicia GBoxGrid
	IniciaGBoxGrid();
	// reabilita Actions
	ActionAlteraPadrao->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::LeConfiguracao(void)
{
	// vari�veis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// l� configura��o de cor de fundo
	redegraf->CorFundo = panelFundoCor->Color;
	// l� configura��o de cor de eqpto.isolado
	grafico->Padrao->isolado.cor = panelIsoladoCor->Color;
	// l� configura��es de barra
	grafico->Padrao->barra.cor = panelBarCor->Color;
	grafico->Padrao->barra.dim = cboxBarDim->Text.ToInt();
	// l� configura��es de chave
	grafico->Padrao->chave.dim = cboxChvDim->Text.ToInt();
	grafico->Padrao->chave.op_cor = RadioGroupChvCor->ItemIndex;
	// l� configura��es de trafo //DVK 2014.12.09
	grafico->Padrao->trafo.dim = cboxTrfDim->Text.ToInt();
	// l� configura��es de trecho
	grafico->Padrao->ligacao.dim = cboxLigDim->Text.ToInt();
	// l� configura��o da cor da legenda
//	grafico->Padrao->legenda.cor = panelLegendaCor->Color;
//    grafico->Padrao->legenda.cor_rede = (RadioGroupLegenda->ItemIndex == 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::Moldura(VTEqpto *eqpto)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// exibe moldura no Eqpto indicado
	grafico->Moldura(eqpto);
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFormPadrao::RadioGroupComentarioEstiloClick(TObject *Sender)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisCOM  = figuras->LisComentario();

   //redefine estilo de todos os comentarios
   for (int n = 0; n < lisCOM->Count; n++)
      {
      comentario = (VTFiguraComentario*)lisCOM->Items[n];
      switch(RadioGroupComentarioEstilo->ItemIndex)
         {
         case 0:  comentario->Formato = fmtRETANGULO; break;
         case 1:  comentario->Formato = fmtELIPSE;    break;
         case 2:  comentario->Formato = fmtFLAG;      break;
         default: comentario->Formato = fmtRETANGULO; break;
         }
      }
   //atualiza o gr�fico
   grafico->Redraw();
   }

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::RadioGroupLegendaClick(TObject *Sender)
{
	//verifica op��o marcada para mostrar/esconder gboxCorLegenda
	gboxCorLegenda->Visible = (RadioGroupLegenda->ItemIndex == 1);
	grafico->Padrao->legenda.cor_rede = (RadioGroupLegenda->ItemIndex == 0);
	//redesenha
    grafico->Redraw();
}

//---------------------------------------------------------------------------
void __fastcall TFormPadrao::tviewChange(TObject *Sender, TTreeNode *Node)
{
	// vari�veis locais
	TTreeNode *raiz;

	// cancela moldura
	Moldura();
	// prote��o
	if (Node == NULL)
		return;
	// esconde GroupBox atualmente vis�vel
	if (gbox_atual)
		gbox_atual->Visible = false;
	// determina TreeNode raiz
	for (raiz = Node; raiz->Level > 0; raiz = raiz->Parent);
	// verifica se Node raiz � o de configura��o geral
	if (raiz == node_raiz.geral)
	{ // exibe GroupBox de configura��o geral
		if ((Node->Level == 1) && (Node->Data != NULL))
		{
			gbox_atual = (TGroupBox*)Node->Data;
			gbox_atual->Align = alClient;
			gbox_atual->Visible = true;
			return;
		}
	}
	// verifica se Node raiz � o de configura��o Reducoes
	if (raiz == node_raiz.reducao)
	{ // exibe GroupBox de configura��o de atributos
		if ((Node->Level >= 0) && (Node->Data != NULL))
		{ // exibe GBoxEqpto
			gbox_atual = gboxEqpto;
			gbox_atual->Align = alClient;
			gbox_atual->Visible = true;
			// atualiza GBoxEqpto c/ atributos do Eqpto associado ao TreeNode
			IniciaCLBoxEqpto((VTEqpto*)Node->Data, Node->Level == 0);
		}
		return;
	}
	// verifica se Node raiz � o de configura��o de atributos
	if (raiz == node_raiz.eqpto)
	{ // exibe GroupBox de configura��o de Eqpto
		if ((Node->Level >= 1) && (Node->Data != NULL))
		{ // exibe GBoxEqpto
			gbox_atual = gboxEqpto;
			gbox_atual->Align = alClient;
			gbox_atual->Visible = true;
			// atualiza GBoxEqpto c/ atributos do Eqpto associado ao TreeNode
			IniciaCLBoxEqpto((VTEqpto*)Node->Data, Node->Level == 1);
			// exibe moldura
			if (Node->Level == 2)
				Moldura((VTEqpto*)Node->Data);
		}
		return;
	}
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormPadrao::TreeViewExisteEqpto(VTEqpto *eqpto)
{
	TTreeNode *node_eqpto;
	TTreeNode *node_tipo;

	// verifica se j� existe um Node filho p/ este tipo de Eqpto
	for (int n = 0; n < node_raiz.eqpto->Count; n++)
	{
		node_tipo = node_raiz.eqpto->Item[n];
		// verifica se TreeNode de tipo � igual ao do Eqpto
		if (node_tipo->Text.CompareIC(eqpto->TipoAsString()) == 0)
		{ // procura Eqpto no TreNode do tipo
			for (int n = 0; n < node_tipo->Count; n++)
			{
				node_eqpto = node_tipo->Item[n];
				if (node_eqpto->Data == eqpto)
					return (node_eqpto);
			}
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormPadrao::TreeViewExisteTipoEqpto(VTEqpto *eqpto)
{
	// vari�veis locais
	TTreeNode *node_eqpto;
	TTreeNode *node_tipo;

	// verifica se j� existe um Node filho p/ este tipo de Eqpto)
	for (int n = 0; n < node_raiz.eqpto->Count; n++)
	{
		node_tipo = node_raiz.eqpto->Item[n];
		if (node_tipo->Text.CompareIC(eqpto->TipoAsString()) == 0)
			return (node_tipo);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::TreeViewInicia(void)
{
	// vari�veis locais
	int tipo_eqpto[] =
	{eqptoBARRA, eqptoCAPACITOR, eqptoCAPSERIE, eqptoCARGA, eqptoCHAVE, eqptoFILTRO, eqptoGERADOR,
		eqptoMEDIDOR, eqptoMUTUA, eqptoREATOR, eqptoREGULADOR, eqptoSUPRIMENTO, eqptoTRAFO,
		eqptoTRAFO3E, eqptoTRAFOZZ, eqptoTRECHO};
	VTEqpto *eqpto;
	VTMRede *mrede;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisMR = redes->LisMRede();

	// reinicia TView
	tview->Items->Clear();
	// cria TreeNodes p/ configura��o geral
	node_raiz.geral = tview->Items->Add(NULL, "Geral");
	tview->Items->AddChildObject(node_raiz.geral, "Cor de Fundo", gboxFundo);
	tview->Items->AddChildObject(node_raiz.geral, "Eqpto.Isolado", gboxIsolado);
	tview->Items->AddChildObject(node_raiz.geral, "Grid", gboxGrid);
	tview->Items->AddChildObject(node_raiz.geral, "Legenda", gboxLegenda);
	tview->Items->AddChildObject(node_raiz.geral, "Barras", gboxBarra);
	tview->Items->AddChildObject(node_raiz.geral, "Chaves", gboxChave);
	// DVK 2014.12.09
	tview->Items->AddChildObject(node_raiz.geral, "Trafos", gboxTrafo);
	tview->Items->AddChildObject(node_raiz.geral, "Trechos", gboxTrecho);
    tview->Items->AddChildObject(node_raiz.geral, "Coment�rios",     gboxComentario);
	// cria TreeNodes p/ Reducoes: monta lista de Redes reduzidas
	lisEQP->Clear();
	for (int n = 0; n < lisMR->Count; n++)
	{
		mrede = (VTMRede*)lisMR->Items[n];
		if (!mrede->Carregada)
			CopiaTList(mrede->LisReducao(), lisEQP);
	}
	if (lisEQP->Count > 0)
	{ // insere TreeNodes p/ Reducoes
		node_raiz.reducao = tview->Items->AddChildObject(NULL, "Redes Reduzidas", lisEQP->First());
		TreeViewIniciaEqpto(node_raiz.reducao, lisEQP);
	}
	// cria TreeNodes p/ Tipos de Eqptos
	node_raiz.eqpto = tview->Items->Add(NULL, "Equipamentos");
	for (int n = 0; n < sizeof(tipo_eqpto) / sizeof(int); n++)
	{ // obt�m lista de Eqptos
		lisEQP->Clear();
		redes->LisEqpto(lisEQP, tipo_eqpto[n]);
		if (lisEQP->Count == 0)
			continue;
		// obt�m primeiro Eqpto da lista p/ determina seu tipo
		eqpto = (VTEqpto*)lisEQP->First();
		// cria TreeNode p/ tipo do Eqpto e insere Eqpto de refer�ncia no TreeNode
		tview->Items->AddChildObject(node_raiz.eqpto, eqpto->TipoAsString(), eqpto);
	}
	// seleciona Node raiz de dados gerais
	tview->Selected = node_raiz.geral;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::TreeViewIniciaEqpto(TTreeNode *node_pai, TList *lisEQP)
{
	// vari�veis locais
	VTEqpto *eqpto;

	// verifica se a lista de Eqptos est� vazia
	if (lisEQP->Count == 0)
		return;
	// insere um TreeNode p/ cada Eqpto
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		// cria um TreeNode p/ o Eqpto
		tview->Items->AddChildObject(node_pai, eqpto->Codigo, eqpto);
	}
	// ordena em ordem alfab�tica os Eqptos dentro do TreeNode de tipo
	node_pai->AlphaSort();
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormPadrao::TreeViewInsereEqpto(VTEqpto *eqpto)
{
	// vari�veis locais
	TTreeNode *node_tipo, *node_eqpto;

	// verifica se existe um TreeNode p/ o tipo do Eqpto
	if ((node_tipo = TreeViewExisteTipoEqpto(eqpto)) == NULL)
	{ // insere Node de n�vel 1: tipo de Eqpto
		node_tipo = tview->Items->AddChild(node_raiz.eqpto, eqpto->TipoAsString());
		// ordena alfabeticamente Nodes dos tipos de Eqptos dentro do seu Node pai
		node_raiz.eqpto->AlphaSort();
	}
	// prote��o
	if (node_tipo == NULL)
		return (NULL);
	// insere Node de n�vel 2: Eqpto
	node_eqpto = tview->Items->AddChildObject(node_tipo, eqpto->Codigo, eqpto);
	// ordena alfabeticamente Nodes dos Eqptos dentro do seu Node pai
	node_tipo->AlphaSort();
	return (node_eqpto);
}

//---------------------------------------------------------------------------
void __fastcall TFormPadrao::tviewDblClick(TObject *Sender)
{
	// vari�veis locais
	TTreeNode *raiz;
	VTEqpto *eqpto;

	// verifica se existe um TreeNode selecionado de Level 3
	if (tview->Selected == NULL)
		return;
	if (tview->Selected->Level != 2)
		return;
	// determina TreeNode raiz
	for (raiz = tview->Selected; raiz->Level > 0; raiz = raiz->Parent);
	// verifica se Node raiz � o de configura��o de atributos de Eqpto
	if (raiz == node_raiz.eqpto)
	{ // redefine �rea de zoom p/ destacar Eqpto
		Zoom((VTEqpto*)tview->Selected->Data);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPadrao::Zoom(VTEqpto *eqpto)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona �rea de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
