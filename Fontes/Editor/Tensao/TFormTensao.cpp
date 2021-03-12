//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormTensao.h"
#include "VTTensao.h"
#include "..\VTEdt.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Explorer\VTAtributo.h"
#include "..\..\Forms\TFormEdtTxt.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Zona.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormTensao::TFormTensao(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent)
	: TFormEdt(Owner, parent)
	{
	//salva ponteiro p/ objetos
	this->apl          = apl;
	this->ActionOnDone = ActionOnDone;
	//cria objetos locais
	atributo = NewObjAtributo(apl);
	zonas    = DLL_NewObjZonas(apl);
	//cria lista de Eqptos selecionados
	lisEQP   = new TList();
	//inicia TView com as Zonas
	TViewInicia();
	//executa AcitoEdt p/ cria objeto edt
	ActionEdt->Execute();
	}

//---------------------------------------------------------------------------
__fastcall TFormTensao::~TFormTensao(void)
	{
	//apaga molduras
	ExibeMoldura();
	//sinaliza t�rmino
	if (ActionOnDone) ActionOnDone->OnExecute(this);
	//destr�i objetos
	if (edt)      {delete edt;      edt      = NULL;}
	if (atributo) {delete atributo; atributo = NULL;}
	if (zonas)    {delete zonas;    zonas    = NULL;}
	//destr�i lista sem destruir seus objetos
	if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ActionEdtExecute(TObject *Sender)
	{
	try{//se necess�rio, cria um objeto VTEdt
		if (edt != NULL) delete edt;
		//cria VTEdt p/ controle do mouse
		edt = NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		//atualiza indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
		} catch (Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ActionEdtTensaoExecute(TObject *Sender)
	{
	//vari�veis locais
	//int         zona_id;
	double      vff_kv;
	TFormEdtTxt *FormEdtTxt;
	VTZona      *zona;

   //FKM - 2015.10.08 Verifica se desconsidera as chaves abertas
   if(!CheckBoxChaveAberta->Checked)
      {
      Aviso("Marque a op��o \"Incluir chaves abertas...\" \nAntes de fazer a altera��o.");
      return;
      }
   //FKM - 2015.10.08
	//determina ZOna selecionada
	if ((zona = TViewZonaSelecionada()) == NULL)
		{
		Aviso("Selecione a regi�o cuja tens�o ser� redefinida");
		return;
		}
	//cria FormEdtTxt
	FormEdtTxt = new TFormEdtTxt(this, "Tens�o da Regi�o");
	FormEdtTxt->Label->Caption = "Vff (kV)";
	FormEdtTxt->Texto = DoubleToStr("%4.3f", zona->Vff_kv);
	if (FormEdtTxt->ShowModal() == mrOk)
		{//l� o novo valor da tens�o
		try{
			vff_kv = StrToDouble(FormEdtTxt->Texto);
			//redefine tens�o dos Eqptos da Zona
			if (RedefineTensao(zona, vff_kv))
				{//atualiza Text do Node
				//zona_id = (zonas->LisZona())->IndexOf(zona) + 1;
				//TViewAtualizaTreeNodeText(TView->Selected, zona, zona_id);
				//atualiza SGrid
				//SGridAtualizaZona(zona, zona_id);
            //reinicia TView
            TViewInicia();
				}
			}catch(Exception &e)
				{
				Aviso("Valor de tens�o inv�lido");
				}
		}
	//destr�i FormEdtTxt
	delete FormEdtTxt;
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ActionFechaExecute(TObject *Sender)
	{
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ActionSelEqptoExecute(TObject *Sender)
   {
	//vari�veis locais
	TTreeNode *node;
	VTEqpto   *eqpto;
	VTZona    *zona;
	TList     *lisEXT = (TList*)Sender;

	//prote��o
	if (lisEXT == NULL) return;
	//determina Eqpto selecionado
	if (lisEXT->Count == 0) return;
	eqpto = (VTEqpto*)lisEXT->First();
	if (eqpto->TipoBarra())
		{//determina Zona da Barra
		if ((zona = zonas->ExisteZona((VTBarra*)eqpto)) != NULL)
			{//determina TreeNode da Zona
			if ((node = TViewExisteTreeNode(zona)) != NULL)
				{//selecione TreeNode
				TView->Selected = node;
				}
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::CheckBoxChaveAbertaClick(TObject *Sender)
	{
	//reinicia o TView
	TViewInicia();
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ExibeMoldura(VTEqpto *eqpto)
	{
	//vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se foi definido um Eqpto
	if (eqpto == NULL)
		{//apaga moldura
		grafico->Moldura();
		return;
		}
	//verifica o tipo do Eqpto
	if      (eqpto->TipoBarra())   grafico->Moldura(eqpto);
	else if (eqpto->TipoEqbar())   grafico->Moldura(eqpto);
	else if (eqpto->TipoLigacao()) grafico->Moldura(eqpto);
	else                           grafico->Moldura();
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::ExibeMoldura(TList *lisEQP)
	{
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormTensao::FormClose(TObject *Sender, TCloseAction &Action)
	{
	//destr�i este Forme
	Action = caFree;
	}

//---------------------------------------------------------------------------
bool __fastcall TFormTensao::RedefineTensao(VTZona *zona, double vff_kv)
	{
	//vari�veis locais
	VTTensao *tensao;

	try{//redefine tens�o da Zona
		zona->RedefineTensao(vff_kv);
		//cria objeto Tensao
		tensao = NewObjTensao(apl);
		tensao->Executa(zona, vff_kv);
		//destr�i objeto
		delete tensao;
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormTensao::SGridAtualiza(VTEqpto *eqpto)
   {
   //vari�veis locais
   int        row;
	int        num_atr;
   AnsiString nome, valor;

   //prote��o
   if (eqpto == NULL)
      {//esconde o Sgrid
		gboxEqpto->Visible = false;
      return;
      }
   //exibe GBoxEqpto
   gboxEqpto->Visible = true;
   //atualiza Caption do GBoxEqpto
	gboxEqpto->Caption = eqpto->TipoAsString() + ": " + eqpto->Codigo;
   //obt�m os atributos do Eqpto
	atributo->Executa(eqpto);
   //for (int nc = 0; nc < sgrid->ColCount; nc++) sgrid->Cols[nc]->Clear();
   //verifica se h� atributos a serem mostrados no SGrid
   if ((num_atr = atributo->Count()) == 0)
		{//esconde o Sgrid
      gboxEqpto->Visible = false;
      return;
      }
   //exibe SGrid
   gboxEqpto->Visible = true;
	//preenche os t�tulos das duas colunas
   sgrid->Cells[0][0] = "Atributo";
   sgrid->Cells[1][0] = "Valor";
   //preenche o SGrid c/ atributos do Eqpto
   row = 1;
	for (int n = 0; n < num_atr; n++)
		{
		atributo->NomeValor(n, nome, valor);
		//verifica se valores em branco
      if (nome.IsEmpty() && valor.IsEmpty()) continue;
      sgrid->Cells[0][row]   = nome;
      sgrid->Cells[1][row++] = valor;
      }
   //redefine n�mero de linhas do SGrid
	sgrid->RowCount = row;
	//seleciona primeira celula
   sgrid->Row = 1; sgrid->Col = 0;
   }


//---------------------------------------------------------------------------
void __fastcall TFormTensao::SGridAtualizaZona(VTZona *zona, int zona_id)
	{
	//vari�veis locais
	AnsiString txt;
	int        row;
	double     v_kv, ang_grau;
	VTFases    *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //prote��o
   if (zona == NULL)
      {//esconde o Sgrid
		gboxEqpto->Visible = false;
      return;
      }
   //atualiza Caption do GBoxEqpto
   gboxEqpto->Caption = "Regi�o " + IntToStr(zona_id);
	//exibe GBoxEqpto
	gboxEqpto->Visible = true;
	//preenche os t�tulos das duas colunas
	sgrid->Cells[0][0] = "Atributo";
	sgrid->Cells[1][0] = "Valor";
	//tipo
	row = 1;
   switch (zona->Tipo)
      {
      case zonaTRIFASICA:     txt = "Trif�sica";     break;
      case zonaMONOFASICA:    txt = "Monof�sica";    break;
      case zonaDELTA_ABERTO:  txt = "Delta Aberto";  break;
		case zonaDELTA_FECHADO: txt = "Delta Fechado"; break;
      default:                txt = "Indefinida";    break;
      }
   sgrid->Cells[0][row]   = "Tipo";
   sgrid->Cells[1][row++] = txt;
   if (zona->Tipo != zonaINDEFINIDA)
		{//defasagem
      sgrid->Cells[0][row]   = "Def.(grau)";
      sgrid->Cells[1][row++] = DoubleToStr("%2.1f", zona->Defasagem_grau);
      //fases
      sgrid->Cells[0][row]   = "Fases";
      sgrid->Cells[1][row++] = (fases->TagToFase[zona->Fases])->codigo;
		//tens�o de linha
      sgrid->Cells[0][row]   = "Vff (kV)";
      sgrid->Cells[1][row++] = DoubleToStr("%4.3f", zona->Vff_kv);
      //tens�o da fase A
      if (zona->Fases & faseA)
         {
			sgrid->Cells[0][row]   = "Van (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_A] , ang_grau = zona->T_grau[indFASE_A]);
         }
      //tens�o da fase B
      if (zona->Fases & faseB)
         {
			sgrid->Cells[0][row]   = "Vbn (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_B], ang_grau = zona->T_grau[indFASE_B]);
         }
      //tens�o da fase C
      if (zona->Fases & faseC)
         {
			sgrid->Cells[0][row]   = "Vcn (kV)";
         sgrid->Cells[1][row++] = txt.sprintf("%4.3f [%4.0f]", v_kv = zona->Vfn_kv[indFASE_C], ang_grau = zona->T_grau[indFASE_C]);
         }
      }
   //n�mero de Barras
   sgrid->Cells[0][row]   = "Barras";
	sgrid->Cells[1][row++] = IntToStr((zona->LisBarra())->Count);
   //define n�mero de linhas do SGrid
	sgrid->RowCount = row;
   //seleciona primeira celula
   sgrid->Row = 1; sgrid->Col = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTensao::TViewAtualizaTreeNodeText(TTreeNode *node, VTZona *zona, int zona_id)
	{
	//vari�veis locais
	AnsiString txt;
	int        num_bar;

	num_bar = (zona->LisBarra())->Count;
	txt.sprintf("Regi�o %d: Vff=%4.3f kV (nb=%d)", zona_id, zona->Vff_kv, num_bar);
	node->Text = txt;
	}

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormTensao::TViewExisteTreeNode(void *data)
	{
   //vari�veis locais
   TTreeNode *node;

	for (int n = 0; n < TView->Items->Count; n++)
      {
		node = TView->Items->Item[n];
      if (node->Data == data) return(node);
      }
   return(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::TViewChange(TObject *Sender, TTreeNode *Node)
	{
	//vari�veis locais
	int zona_id;
	VTZona *zona;

	//verifica TreeNode selecionado
	switch(TView->Selected->Level)
		{
		case 0: //apaga conte�do do SGrid
			SGridAtualiza(NULL);
			break;
		case 1:  //Zona
			zona    = (VTZona*)TView->Selected->Data;
			zona_id = (zonas->LisZona())->IndexOf(zona) + 1;
			SGridAtualizaZona(zona, zona_id);
			//destaca todas as Barras
			ExibeMoldura(zona->LisBarra());
			break;
		case 2:  //Barra da Zona:
			SGridAtualiza((VTEqpto*)Node->Data);
			//exibe Moldura
			ExibeMoldura((VTEqpto*)Node->Data);
			break;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormTensao::TViewInicia(void)
	{
	//vari�veis locais
	TTreeNode *node_raiz, *node_zona;
	TList     *lisBAR;
	VTBarra   *barra;
	VTZona    *zona;
	TList     *lisZONA = zonas->LisZona();

	//reinicia Zonas
	zonas->ChaveAbertaEnabled = CheckBoxChaveAberta->Checked;
	zonas->AvisoEnabled       = true;
	zonas->Executa();
	//reinicia TView
	TView->Items->Clear();
	//insere TreeNode raiz
	node_raiz = TView->Items->Add(NULL, "Regi�o");
	//insere TreeNodes p/ Zonas
	for (int n = 0; n < lisZONA->Count; n++)
		{
		zona   = (VTZona*)lisZONA->Items[n];
		lisBAR = zona->LisBarra();
		//insere TreeNode p/ Zona
		node_zona = TView->Items->AddChildObject(node_raiz, "", zona);
		//atualiza Text do Node
		TViewAtualizaTreeNodeText(node_zona, zona, n+1);
		//insere TReeNodes p/ Barras das Zonas
		for (int nb = 0; nb < lisBAR->Count; nb++)
			{
			barra = (VTBarra*)lisBAR->Items[nb];
			//insere TreeNode p/ barra
			TView->Items->AddChildObject(node_zona, barra->Codigo, barra);
			}
		}
	//seleciona Node raiz
	node_raiz->Expand(false);
	TView->Selected = node_raiz;
	}

//---------------------------------------------------------------------------
VTZona* __fastcall TFormTensao::TViewZonaSelecionada(void)
	{
	//vari�veis locais
	TTreeNode *node = TView->Selected;

	//prote��o
	if (node == NULL) return(NULL);
	while (node->Level > 1) node = node->Parent;
	if (node->Level != 1) return(NULL);
	//determina Zona do Node
	return((VTZona*)node->Data);
	}

//---------------------------------------------------------------------------
//eof


