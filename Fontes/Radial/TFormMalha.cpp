//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMalha.h"
#include "VTMalha.h"
#include "VTMalhas.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMalha::TFormMalha(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
	{
	//varia´veis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

	//salva ponteiro para objetos
	this->apl = apl_owner;
	//inclui Action a ser executada quando a rede for alterada
	edita->ActionOnChangeRedeInsere(ActionRedeChanged);
	//insere Form em seu Parent
	if (parent) Parent = parent;
   //cria objeto VTMalhas
	malhas = NewObjMalhas(apl);
	//identifica malhas nas Redes
	malhas->Inicia();
	//inicia ListView com as malhas internas, externas e de geração
	LViewInicia(LViewMalhaInterna, malhas->LisMalhaInterna());
	LViewInicia(LViewMalhaExterna, malhas->LisMalhaExterna());
	LViewInicia(LViewMalhaGeracao, malhas->LisMalhaGeracao());
	//DVK 2014.12.06 mostra form
	this->Visible = true;
	//seleciona TabSheet
	if      (LViewMalhaInterna->Items->Count > 0) PageControl->ActivePage = TabSheetMalhaInterna;
	else if (LViewMalhaExterna->Items->Count > 0) PageControl->ActivePage = TabSheetMalhaExterna;
	else if (LViewMalhaGeracao->Items->Count > 0) PageControl->ActivePage = TabSheetMalhaGeracao;
	else                                          PageControl->ActivePage = TabSheetMalhaInterna;
	//simula alteração de TabSheet para atualizar o gráfico com a malha selecionada
	PageControlChange(PageControl);
	}

//---------------------------------------------------------------------------
__fastcall TFormMalha::~TFormMalha(void)
   {
	//variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	//retira Action a ser executada quando a rede for alterada
	edita->ActionOnChangeRedeRetira(ActionRedeChanged);
	//salva parâmetros do Form //DVK 2014.12.06
	if (geral) geral->Infoset->SaveToFile(this);
	//apaga eventual moldura
   DestacaEqpto();
   //destrói objetos
   if (malhas) {delete malhas; malhas = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMalha::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMalha::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Análise_de_Malhas");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormMalha::ActionRedeChangedExecute(TObject *Sender)
	{
	//identifica malhas nas Redes
	malhas->Inicia();
	//inicia ListView com as malhas internas, externas e de geração
	LViewInicia(LViewMalhaInterna , malhas->LisMalhaInterna());
	LViewInicia(LViewMalhaExterna , malhas->LisMalhaExterna());
	LViewInicia(LViewMalhaGeracao , malhas->LisMalhaGeracao());
	//DVK 2015.06.08 atualiza gráfico conforme tabsheet selecionado
	PageControlChange(PageControl);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::ActionSelColorExecute(TObject *Sender)
	{
	//variáveis locais
	TListView *LView;
	VTMalha   *malha;

	//esconde este form
	Visible = false;
	ColorDialog->Color = PanelColor->Color;
	if (ColorDialog->Execute())
		{//redefine cor do PanelColor
		PanelColor->Color = ColorDialog->Color;
		//determina Malha selecionada
		if      (PageControl->ActivePage == TabSheetMalhaExterna) LView = LViewMalhaExterna;
		else if (PageControl->ActivePage == TabSheetMalhaInterna) LView = LViewMalhaInterna;
		else if (PageControl->ActivePage == TabSheetMalhaGeracao) LView = LViewMalhaGeracao;
		else                                                      return;
		//determina Malha selecionada
		if (LView->Selected == NULL) return;
		malha = (VTMalha*)LView->Selected->Data;
		//torna a destacar os equipamentos da malha selecionada
		if (malha != NULL) DestacaEqpto(malha->LisLigacao());
		}
	//exibe o form
	Visible = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::CheckBoxMalhaClick(TObject *Sender)
	{
	//DVK 2015.06.08 limpa gráfico
	if(! CheckBoxMalha->Checked) DestacaEqpto(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::DestacaEqpto(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->DestacaEqpto(lisEQP, PanelColor->Color, 2);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormMalha::FiltraSeparador(AnsiString valor, char separador)
	{//DVK 2015.06.08
   //variáveis locais
   AnsiString txt;
   int        len = valor.Length();

   //descarta ponto (separador de milhar)
   for (int n = 1; n <= len; n++)
      {
      if (valor[n] != separador) txt += valor[n];
      }
	return(txt);
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMalha::FormShow(TObject *Sender)
	{//DVK 2014.12.06
	//variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
		{//lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::LViewColumnClick(TObject *Sender, TListColumn *Column)
	{//DVK 2015.06.08
	//verifica se selecionou a mesma coluna
	if ((LViewSort.lview == Sender)&&(LViewSort.column == Column))
		{//inverte ordem
		LViewSort.ascendente = ! LViewSort.ascendente;
		}
	else
		{//redefine índice da coluna usada p/ ordenação
		LViewSort.lview      = (TListView*)Sender;
		LViewSort.column     = Column;
		LViewSort.ascendente = true;
		}

	//força reordenação
	LViewSort.lview->AlphaSort();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::LViewCompare(TObject *Sender, TListItem *Item1,
													  TListItem *Item2, int Data, int &Compare)
	{//DVK 2015.06.08
	//variáveis locais
	int       index = LViewSort.column->Index - 1;
	double    valor1, valor2;

	//verifica se Item1 refere-se a uma alinha de total
	if (Item1->Caption.CompareIC("total") == 0)
		{//mantém Item1 no final do ListView
		Compare = 1;
		return;
		}
	//verifica se Item2 refere-se a uma linha de total
	if (Item2->Caption.CompareIC("total") == 0)
		{//mantém Item2 no final do ListView
		Compare = -1;
		return;
		}
	//verifica Coluna sendo ordenada
	if (LViewSort.column->Index == 0)
		{//compara Caption
		Compare = CompareText(Item1->Caption,Item2->Caption);
		}
	//verifica Coluna possui string (Tag é definido na edição do ListView)
	else if (LViewSort.column->Tag == 0)
		{//comparação de colunas com string
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		}
	else
		{//comparação de colunas com valor numérico
		valor1  = StrToDouble(FiltraSeparador(Item1->SubItems->Strings[index]));
		valor2  = StrToDouble(FiltraSeparador(Item2->SubItems->Strings[index]));
		if      (valor1 < valor2) Compare = -1;
		else if (valor1 > valor2) Compare =  1;
		else                      Compare =  0;
		}
	//verifica se ordem ascendente/descendente
	if (! LViewSort.ascendente) Compare = - Compare;
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::LViewDblClick(TObject *Sender)
	{
	//variáveis locais
	VTEqpto   *eqpto;
	VTMalha   *malha;
	TListView *LView = LViewMalhaInterna;

	//verifica se há uma Malha selecionado
	if (LView->ItemIndex < 0) return;
	 malha = (VTMalha*)(LView->Items->Item[LView->ItemIndex]->Data);
	 if (malha->NumeroDeLigacoes() > 0)
		{
		eqpto = (VTEqpto*)(malha->LisLigacao()->First());
		//redefine área de zoom p/ destacar Eqpto
		Zoom(eqpto);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::LViewInicia(TListView *LView , TList *lisMALHA)
	{
	//variáveis locais
	AnsiString txt;
	TListItem *Item;
	VTMalha   *malha;
	VTRede    *rede;
	TList     *lisREDE;

	//reinicia ListView
	LView->Clear();
	//preenche ListView com as Malhas de lisMALHA
	for (int n = 0; n < lisMALHA->Count; n++)
		{
		malha         = (VTMalha*)lisMALHA->Items[n];
		lisREDE       = malha->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
			{
			rede          = (VTRede*)lisREDE->Items[nr];
			Item          = LView->Items->Add();
			Item->Data    = malha;
			Item->Caption = IntToStr(n+1);
			//verifica se rede carregada
			if (rede->Carregada)
				{
				Item->SubItems->Add(rede->Codigo);
				Item->SubItems->Add(IntToStr(malha->NumeroDeLigacoes(rede)));
				Item->SubItems->Add(IntToStr(malha->NumeroDeLigacoes(rede, eqptoCHAVE)));
				}
			else
				{//verifica se a Rede está associada a um Cluster
				if (rede->Cluster) txt = rede->Cluster->Codigo + ": " + rede->Codigo;
				else               txt = rede->Codigo;
				Item->SubItems->Add(txt +  + " (eqv.)");
				Item->SubItems->Add("-");
				Item->SubItems->Add("-");
				}
			}
		}
	//pré-seleciona primeira Malha
	if (LView->Items->Count > 0) LView->Selected = LView->Items->Item[0];
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
	{
	//variáveis locais
	VTMalha *malha;

	//proteção
	if (Item == NULL)
		{//cancela destaque da malha
		DestacaEqpto();
		return;
		}
	//verifica se o Item não está selecionado
	if (! Selected) return;
	//determina Malha associada ao Item selecionado
	if ((malha = (VTMalha*)Item->Data) == NULL) return;
	//verifica se a exibição gráfica está habilitada
	if (CheckBoxMalha->Checked)
		{//destaca Ligacoes da Malha
		DestacaEqpto(malha->LisLigacao());
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::PageControlChange(TObject *Sender)
	{
	//simmula seleção de Malha para atualiza o gráfico
	if (PageControl->ActivePage == TabSheetMalhaInterna)
		{
		LViewSelectItem(LViewMalhaInterna, LViewMalhaInterna->Selected, true);
		}
	else if (PageControl->ActivePage == TabSheetMalhaExterna)
		{
		LViewSelectItem(LViewMalhaExterna, LViewMalhaExterna->Selected, true);
		}
	else if (PageControl->ActivePage == TabSheetMalhaGeracao)
		{
		LViewSelectItem(LViewMalhaGeracao, LViewMalhaGeracao->Selected, true);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormMalha::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //proteção
   if (eqpto == NULL) return;
   //seleciona área de zoom
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof


//---------------------------------------------------------------------------



