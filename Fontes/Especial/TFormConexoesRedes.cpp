// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormConexoesRedes.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Report\VTReport.h"
#include "..\Zona\VTZona.h"
#include "..\Zona\VTZonas.h"

// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Report.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormConexoesRedes::TFormConexoesRedes(TComponent* Owner, VTApl *apl_owner)
	: TForm(Owner)
{
	// salva ponteiro
	apl = apl_owner;
	// cria lista
	lisEQP = new TList();
	lisLIG = new TList();
	// cria progresso
	progresso = DLL_NewObjProgresso(NULL, this);
	// localiza ligações entre redes
	Executa();
}

// ---------------------------------------------------------------------------
__fastcall TFormConexoesRedes::~TFormConexoesRedes(void)
{
	// destroi lista
	DestroiTObject(lisEQP);
	DestroiTObject(lisLIG);
	// destroi obj
	if (progresso)
	{
		progresso->Stop();
		DestroiTObject(progresso);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::ActionExportarExecute(TObject *Sender)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTReport *report;

	// inicia diretorio
	if (SaveDialog->InitialDir.IsEmpty())
	{
		SaveDialog->InitialDir = path->DirExporta();
	}
	// exporta conteudo do LView
	if (SaveDialog->Execute())
	{
		report = DLL_NewObjReport(apl);
		if (report->ArquivoTexto(SaveDialog->FileName, LViewConexoes, ""))
		{
			Aviso("Arquivo exportado com sucesso");
		}
		else
		{
			Erro("Erro ao exportar");
		}
		delete report;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::ActionFecharExecute(TObject *Sender)
{
	// esconde o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::Executa(void)
{
	// variáveis locais
	TListItem *item;
	VTBarra *barra1, *barra2;
	VTChave *chave;
	VTLigacao *ligacao;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTRede *rede1, *rede2;
	double vnom1, vnom2;

	try
	{ // inicia progresso
		progresso->Start(progDEFAULT);
		//executa zonas sem chave aberta
		zonas->ChaveAbertaEnabled = false;
		zonas->Executa(redes);
		// limpa lista e LView
		lisLIG->Clear();
		LViewConexoes->Clear();
		// lista chaves
		redes->LisLigacao(lisLIG, eqptoCHAVE);

		for (int n = 0; n < lisLIG->Count; n++)
		{
			chave = (VTChave*)lisLIG->Items[n];
			if(chave->Fechada) continue;
			barra1 = chave->pbarra1;
			barra2 = chave->pbarra2;
			vnom1 = vnom2 = 0.;
			if(barra1->zona != NULL)
			{
				vnom1 = barra1->zona->Vff_kv;
			}
			if(barra2->zona != NULL)
			{
				vnom2 = barra2->zona->Vff_kv;
			}
			if(IsDoubleZero(vnom1 - vnom2)) continue;
			/*
			lisEQP->Clear();
			rede1 = rede2 = NULL;
			// todas ligações barra 1
			redes->LisLigacao(lisEQP, barra1);
			for (int k = 0; k < lisEQP->Count; k++)
			{
				ligacao = (VTLigacao*)lisEQP->Items[k];
				if ((rede1 == NULL) && (rede1 != ligacao->rede))
				{
					rede1 = ligacao->rede;
				}
				else if ((rede2 == NULL) && (rede2 != ligacao->rede))
				{
					rede2 = ligacao->rede;
				}
			}
			// todas ligações barra 2
			redes->LisLigacao(lisEQP, barra2);
			for (int k = 0; k < lisEQP->Count; k++)
			{
				ligacao = (VTLigacao*)lisEQP->Items[k];
				if ((rede1 == NULL) && (rede1 != ligacao->rede))
				{
					rede1 = ligacao->rede;
				}
				else if ((rede2 == NULL) && (rede2 != ligacao->rede))
				{
					rede2 = ligacao->rede;
				}
			}
			// somente se houver rede1 diferente de 2
			if (rede1 == rede2)
				continue;
			if (rede1 == NULL)
				continue;
			if (rede2 == NULL)
				continue;
			*/
			// insere na lista
			item = LViewConexoes->Items->Add();
			item->Data = chave;
			item->Caption = "Chave";
			item->SubItems->Add(chave->Codigo);

//			item->SubItems->Add(rede1->Codigo);
//			item->SubItems->Add(rede2->Codigo);
			item->SubItems->Add(DoubleToStr("%3.2f", vnom1));
			item->SubItems->Add(DoubleToStr("%3.2f", vnom2));
			if (chave->Aberta)
			{
				item->SubItems->Add("Aberta");
			}
			else
			{
				item->SubItems->Add("Fechada");
			}
		}
		// para progresso
		progresso->Stop();
	}
	catch (Exception &e)
	{ // para progresso
		progresso->Stop();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::FormClose(TObject *Sender, TCloseAction &Action)
{
	Moldura(NULL);
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::LViewConexoesColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (LViewSort.Column == Column)
	{ // inverte ordem
		LViewSort.OrdemCrescente = !LViewSort.OrdemCrescente;
	}
	else
	{ // salva coluna usada p/ ordenação
		LViewSort.Column = Column;
		LViewSort.OrdemCrescente = true;
	}
	// força reordenação
	((TListView*)Sender)->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::LViewConexoesCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
	// variáveis locais
	int index;
	double valor1, valor2;

	// proteção
	if (LViewSort.Column == NULL)
		return;
	// determina index p/ substrings
	index = LViewSort.Column->Index - 1;
	// verifica se coluna de Caption
	if (LViewSort.Column->Index == 0)
	{ // coluna de Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	else
	{ // coluna com texto
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	// verifica se ordem crescente/decrescente
	if (!LViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::LViewConexoesDblClick(TObject *Sender)
{
	// variáveis locais
	TListItem *item = LViewConexoes->Items->Item[LViewConexoes->ItemIndex];
	VTEqpto *eqpto = (VTEqpto*)item->Data;

	// redefine área de zoom p/ destacar Eqpto
	Zoom(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::LViewConexoesSelectItem(TObject *Sender, TListItem *Item,
	bool Selected)
{
	// variáveis locais
	VTEqpto *eqpto = (VTEqpto*)Item->Data;

	Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	graf->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConexoesRedes::Zoom(VTEqpto *eqpto)
{
	// variáveis locais
	bool ampliar;
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	// proteção
	if (eqpto == NULL)
		return;
	// define se deve ampliar ou diminuir imagem
	if (zoom.shift_state.Contains(ssShift))
		ampliar = false;
	else
		ampliar = true;
	// seleciona área de zoom
	graf->SelecionaZoom(eqpto, ampliar);
}

// ---------------------------------------------------------------------------
