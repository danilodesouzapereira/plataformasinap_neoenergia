//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormFinalizaObra.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\VTObras.h>
#include <PlataformaSinap\Fontes\Obra\ItemObra\VTItemObra.h>
#include <PlataformaSinap\Fontes\Radial\VTArvore.h>
#include <PlataformaSinap\Fontes\Radial\VTNode.h>
#include <PlataformaSinap\Fontes\MultiObra\VTMultiObraBO.h>
#include <PlataformaSinap\Fontes\MultiObra\VTMultiObra.h>
#include <PlataformaSinap\Fontes\MultiObra\VTNodeDetalhe.h>
#include <PlataformaSinap\Fontes\Sinap\VTSinapChild.h>
//---------------------------------------------------------------------------

#include <DLL_Inc\Edita.h>
#include <DLL_Inc\MultiObra.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormFinalizaObra *FormFinalizaObra;
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
__fastcall TFormFinalizaObra::TFormFinalizaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
	: TForm(Owner)
{
	try
	{
		this->apl = apl_owner;
		this->obra_ref = obra;
		this->itemObra_ref = NULL;
		this->multiObra_ref = NULL;
	//	multiObraBO = DLL_NewObjMultiObraBO(apl);
		multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
		multiObra_ref = multiObraBO->AnalisaAcoesObra(obra_ref);
		//escnde as coisas
		this->MontantePNT = false;
		//    this->Empresa = false;
		//	this->ARIES = false;

        //duplica itens da obra
        lisItemObra_orig = new TList();
        ClonaTodosItensObra();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

}
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
__fastcall TFormFinalizaObra::~TFormFinalizaObra(void)
{
//	DestroiTObject(multiObraBO);
    DestroiTObject(lisItemObra_orig);
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormFinalizaObra::ActionCancelaExecute(TObject *Sender)
{
    //destroi multiObra_ref
    multiObraBO->DestroiMultiObra(obra_ref);
    //copia itens obra originais para a obra_ref
    CopiaItensObraOriginal();

    // fecha
    ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionConfirmaExecute(TObject *Sender)
{
	try
	{
		if(ConfirmaObra())
		{
			ModalResult = mrOk;
		}
		else
		{
            //faz nada
        }
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionConfirmaItemExecute(TObject *Sender)
{
	ConfirmaItem();
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionDetalharMaisExecute(TObject *Sender)
{
	VTNodeDetalhe *nodeDet;

	try
	{
		nodeDet = NodeDetalheSelecionado();
		if ((multiObra_ref != NULL) && (nodeDet != NULL))
		{
			multiObra_ref->ExpandeNode(nodeDet);
			SelecionaNodeRaiz();
			PreencheInfoItemObra(NodeDetalheSelecionado());
			TreeViewInicia();
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionDetalharMenosExecute(TObject *Sender)
{
	VTNodeDetalhe *nodeDet;

	try
	{
		nodeDet = NodeDetalheSelecionado();
		if ((multiObra_ref != NULL) && (nodeDet != NULL))
		{
			multiObra_ref->ContraiNode(nodeDet);
			SelecionaNodeRaiz();
			PreencheInfoItemObra(NodeDetalheSelecionado());
			TreeViewInicia();
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionEdtExcluirExecute(TObject *Sender)
{
	bool excluir = true;
	VTItemObra *itemOrig;
	TTreeNode *node;

	try
	{
		if((itemObra_ref != NULL) && (multiObra_ref != NULL))
		{
			//se era pra contabilizar
			excluir = itemObra_ref->Contabilizar;
			itemOrig = (VTItemObra*)itemObra_ref->Object;
			//troca
			multiObra_ref->HabilitaItemObra(itemOrig, !excluir);
			//altera a referencia
			itemObra_ref->CopiaAtributosDe(*itemOrig);
			//altera o butão
			TrocaEstadoForm();
			//desabilita as coisas
	//		EnableDetalheItem(!excluir);
			//guarda o node selecionado
			node = TreeViewItems->Selected;
			//pinta denovo o treeview
			AtualizaIconesTreeView();
	//		TreeViewInicia();
	//		//
	//        SelecionaNodeRaiz();
	//		//seleciona novamente ele
			TreeViewItems->Select(node);

		}
    }catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ActionHabilitaEdtExecute(TObject *Sender)
{
	VTNodeDetalhe *nodeDet;
	VTItemObra *itemObraAux;

	try
	{
		nodeDet = NodeDetalheSelecionado();

		if ((nodeDet != NULL) &&(nodeDet->Habilitado))
		{
			itemObraAux = nodeDet->ItemObra;
			//clona o item para editar o clone
			ClonaItemObraParaEdicaoTemp(itemObraAux);
		}
		if((itemObra_ref != NULL) && (multiObra_ref != NULL))
		{
			//habilita
			multiObra_ref->HabilitaItemObra(itemObraAux, true);
			//altera o butão
	//		EnableDetalheItem(true);
	//		PanelDir->Color = CorPanelDir(NodeDetalheSelecionado());
			TrocaEstadoForm();
			this->Refresh();
			//pinta denovo o treeview
	//		TreeViewInicia();
			AtualizaIconesTreeView();
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormFinalizaObra::ActionSalvaFechaExecute(TObject *Sender)
{
	try
	{
		//guarda na obra os itens
		if (multiObra_ref)
		{
			multiObra_ref->AtualizaEstadosNodes();
			multiObra_ref->FinalizaObra();
		}
        //verifica se há itens pendentes (se a obra poderá ser finalizada)
        if(! multiObraBO->IsObraFinalizada(obra_ref))
        {
            Aviso("Há itens pendentes nesta obra");
        }
		ModalResult = mrCancel;
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}

//---------------------------------------------------------------------------
void   __fastcall TFormFinalizaObra::AtualizaIconesTreeView(void)
{
    TTreeNode *node;

	try
	{
		TreeViewItems->Items->BeginUpdate();
		TreeViewItems->Items->EndUpdate();
		TreeViewItems->Refresh();
	//	TreeView->Repaint();
	//	TreeViewInicia();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void   __fastcall TFormFinalizaObra::AtualizaItemObraRef(VTNodeDetalhe* nodeDet)
{
	VTItemObra *itemObraAux = NULL;

	try
	{
		if ((nodeDet != NULL) &&(nodeDet->Habilitado))
		{
			itemObraAux = nodeDet->ItemObra;
			//clona o item para editar o clone
			ClonaItemObraParaEdicaoTemp(itemObraAux);
		}
		else
		{
			itemObra_ref = NULL;
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObra::CheckBoxQualidadeExit(TObject *Sender)
{
	try
	{
		CheckBoxQualidadeClick(Sender);
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15755250
void __fastcall TFormFinalizaObra::CheckBoxQualidadeClick(TObject *Sender)
{
	try
	{
		// exibe/esconde GBoxTaxaFalha
//		GBoxTaxaFalha->Visible = CheckBoxQualidade->Checked;
		GBoxTaxaFalha->Enabled = CheckBoxQualidade->Checked;
		// arruma tamanho do groupbox
//		if (GBoxTaxaFalha->Visible)
//		{
//			GBoxQualidade->Height = 45 + GBoxTaxaFalha->Height;
//		}
//		else
//		{
//			GBoxQualidade->Height = 45;
//		}
		//ajusta o panel comum
		PanelComumAjuste();
		//altera o objeto
		if (itemObra_ref != NULL)
		{
			itemObra_ref->AlteraTaxaFalha = CheckBoxQualidade->Checked;
		}
		// acerta posição Top do GBoxQualidade com valor salvo em Tag
		// GBoxTaxaFalha->Top = GBoxTaxaFalha->Tag;
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ClonaItemObraParaEdicaoTemp(VTItemObra *item)
{
	try
	{
		if(item != NULL)
		{
			DeletaItemObraRef();
			itemObra_ref = item->Clone(true,false);
			itemObra_ref->Object = item;
	//		item->Object = itemObra_ref;
		}
		else
		{
			DeletaItemObraRef();
			itemObra_ref = NULL;
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormFinalizaObra::ClonaTodosItensObra(void)
{
    VTItemObra *itemObra, *clone;
    bool preserveId = true;
    bool isDeep = false;

    for(int n = 0; n < obra_ref->LisItemObra->Count; n++)
    {//clona todos itens obra para o caso de ter que voltar como era antes
        itemObra = (VTItemObra*)obra_ref->LisItemObra->Items[n];
        clone = itemObra->Clone(preserveId,isDeep);
        lisItemObra_orig->Add(clone);
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ConfirmaItem(void)
{
    VTItemObra *itemObra;

	try
	{
		//valida o item, se passar muda tag
		if (ValidaInputItemObra())
		{
			ConfirmaItemProprio();
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::ConfirmaItemProprio(void)
{
    VTItemObra *itemObra;

	try
	{
        if (itemObra_ref == NULL)
			{return;}
		//debug: id
//		EditId->Text = IntToStr(itemObra_ref->Id);
		//infos comuns
        itemObra_ref->Nome = EditNomeItem->Text;
		//AlteraTaxaFalha
		itemObra_ref->AlteraTaxaFalha = CheckBoxQualidade->Checked;
		//TaxaFalhaInsercao
		itemObra_ref->TaxaFalhaInsercao = StrToDouble(EditTaxaInsercao->Text);
		//TaxaFalhaSubstituicao
		itemObra_ref->TaxaFalhaSubstituicao = StrToDouble(EditTaxaSubstituicao->Text);
		//valida o item, se passar muda tag
//		if(itemObra_ref != NULL)
//		{
			itemObra_ref->Configurado = true;
//		}
		//Efetiva mudanças
		itemObra = (VTItemObra*)itemObra_ref->Object;
		itemObra->CopiaAtributosDe(*itemObra_ref);
		//atualiza o form
		if(multiObra_ref != NULL)
		{
			multiObra_ref->AtualizaEstadosNodes();
//            EnableDetalheItem(false);
//			PanelDir->Color = CorPanelDir(NodeDetalheSelecionado());
			TrocaEstadoForm();
			this->Refresh();
//			TreeViewInicia();
			AtualizaIconesTreeView();
		}
		//destroi o clone
		DeletaItemObraRef();

	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}

// https://sinapsisenergia.teamwork.com/#/tasks/15933980
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ConfirmaObra(void)
{
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	bool sucesso = false;

	try
	{
		//se validar a obra
		if (ValidaInputObra())
		{
			//Limpa os ipe atributos da obra
			if (obra_ref != NULL)
			{
				obra_ref->LisObject->Clear();
			}
			sucesso = ConfirmaObraPropria();
			if (sucesso == true)
			{
				if ((multiObra_ref != NULL) &&(multiObra_ref->TodosItemsConfigurados))
				{
					if(obra_ref != NULL)
					{
						obra_ref->Finalizada = true;
						if(sinap_child != NULL)
						{
						   sinap_child->BeginEditorBlock(obra_ref);
						}
					}
				}
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ConfirmaObraPropria(void)
{
	bool sucesso = true;

	try
	{
		//guarda na obra os itens
		if (multiObra_ref)
		{
			multiObra_ref->AtualizaEstadosNodes();
			multiObra_ref->FinalizaObra();
		}
		else
		{
			sucesso = false;
        }

	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = true;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
void __fastcall TFormFinalizaObra::CopiaItensObraOriginal(void)
{
    //variaveis locais
    VTItemObra *itemObra;

    //retira itens da obra ref
    //LimpaTList(obra_ref->LisItemObra);

    //insere itens originais
    for(int n = 0; n < lisItemObra_orig->Count; n++)
    {
        itemObra = (VTItemObra*)lisItemObra_orig->Items[n];
        obra_ref->AddItemObra(itemObra);
    }
}

//---------------------------------------------------------------------------
TColor __fastcall TFormFinalizaObra::CorPanelDir(VTNodeDetalhe* nodeDet)
{
	TColor cor = clBtnFace;

	try
	{
		if (nodeDet != NULL)
		{
			switch (nodeDet->Estado)
			{
				case eND_CONFIGURADO:
					cor = clMoneyGreen;
					break;
				case eND_NAOCONTABILIZADO:
					cor =  clWebSalmon;
					break;
				default:
					cor = clBtnFace;
					;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return cor;
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::DeletaItemObraRef(void)
{
	VTItemObra *item = NULL;

	try
	{
		if (itemObra_ref != NULL)
		{
			//verifica se tem um parceiro
	//		if (itemObra_ref->Object != NULL)
	//		{
	//			item = (VTItemObra*)itemObra_ref->Object;
	//			item->Object = NULL;
	//		}
			delete itemObra_ref;
			itemObra_ref = NULL;
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditPNTChange(TObject *Sender)
{
	try
	{
		valorPNT = GetValorDouble(EditPNT, "Redução de perdas não técnicas");
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditPNTClick(TObject *Sender)
{
	EditPNT->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditPNTExit(TObject *Sender)
{
    double valor = 0;

	try
	{
		if(itemObra_ref != NULL)
		{
			if (ValidaPNT(valor))
			{
				itemObra_ref->ReducaoPNT = valor;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditTaxaInsercaoClick(TObject *Sender)
{
	if (GBoxTaxaFalha->Enabled)
		EditTaxaInsercao->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditTaxaSubstituicaoClick(TObject *Sender)
{
	if (GBoxTaxaFalha->Enabled)
		EditTaxaSubstituicao->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditTaxaInsercaoExit(TObject *Sender)
{
	double valor = 0;

	try
	{
		if(itemObra_ref != NULL)
		{
			if (ValidaTaxaInsercao(valor))
			{
				itemObra_ref->TaxaFalhaInsercao = valor;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::EditTaxaSubstituicaoExit(TObject *Sender)
{
	double valor = 0;

	try
	{
		if(itemObra_ref != NULL)
		{
			if (ValidaTaxaSubstituicao(valor))
			{
				itemObra_ref->TaxaFalhaSubstituicao = valor;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void   __fastcall TFormFinalizaObra::EnableDetalheItem(bool enable)
{
	try
	{
		EditNomeItem->Enabled = enable;
		CheckBoxQualidade->Enabled = enable;
		EditTaxaSubstituicao->Enabled = enable;
		EditTaxaInsercao->Enabled = enable;
		EditPNT->Enabled = enable;
		ToolButtonConfirma->Visible = enable;
	//	butHabilitaEdt->Visible = !enable;
    }catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
int  __fastcall TFormFinalizaObra::FormHeight(void)
{
	int heightDir,heightEsq;
	int maior = 0;

	try
	{
		heightDir = heightEsq = 0;

		//mede esquerda
		heightEsq = FormHeightEsq();

		//mede direita
		heightDir = FormHeightDir();

		//verifica o maior
		if (heightDir > heightEsq)
		{
			maior = heightDir;
		}
		else
		{
			maior = heightEsq;
		}
		//soma o toobar
		maior += Height(ToolBarForm);
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return maior;
}
//---------------------------------------------------------------------------
int  __fastcall TFormFinalizaObra::FormHeightDir(void)
{
	int heightDir = 0;

	try
	{
		//mede direita
		heightDir += Height(PanelAriesDir);
		heightDir += Height(PanelComum);
		heightDir += Height(PanelEmpresaDir);
		heightDir += Height(ToolBarItem);
		//acerta o groupboxDetalhes
		GroupBoxDetalhes->Height = heightDir;
		heightDir = Height(GroupBoxDetalhes);
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return heightDir;
}
//---------------------------------------------------------------------------
int  __fastcall TFormFinalizaObra::FormHeightEsq(void)
{
	int heightEsq = 0;
	int maior = 0;

	try
	{
		GBoxItems->Align = alTop;
		//mede esquerda
		heightEsq += Height(PanelAriesEsq);
		heightEsq += Height(PanelNome);
		heightEsq += Height(PanelEmpresaEsq);
		heightEsq += Height(GBoxItems);

		GBoxItems->Align = alClient;
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return heightEsq;
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::FormShow(TObject *Sender)
{
	try
	{
		if (itemObra_ref == NULL)
		{
			TreeViewInicia();
			RemoveCaptionsPanels();
			//preenche as infos basicas da obra
			PreencheInfoObra(obra_ref);
			CheckBoxQualidadeClick(NULL);
			PanelComumAjuste();
		//	GroupBoxDetalhes->Visible = false;

			this->ClientHeight = FormHeight();
			this->Refresh();
			//seleciona o node raiz
			SelecionaNodeRaiz();
			PreencheInfoItemObra(NodeDetalheSelecionado());
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormFinalizaObra::GetNome(void)
{
	return (EditNome->Text.Trim());
}
// ---------------------------------------------------------------------------
double __fastcall TFormFinalizaObra::GetReducaoPNT(void)
{
	return (valorPNT);
}
// ---------------------------------------------------------------------------
double __fastcall TFormFinalizaObra::GetValorDouble(TEdit *Edit, AnsiString descricao)
{
	try
	{
		return (StrToDouble(Edit->Text));
	}
	catch (Exception &e)
	{
		Erro(descricao + " com valor inválido");
		return (0.);
	}
}
//---------------------------------------------------------------------------
int  __fastcall TFormFinalizaObra::Height(TWinControl *control)
{
	int height = 0;

	try
	{
		if (control->Visible)
		{
			if (control->AlignWithMargins)
			{
				height += control->Margins->Top;
				height += control->Margins->Bottom;
			}
			height += control->Height;
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return height;
}
//---------------------------------------------------------------------------
int  __fastcall TFormFinalizaObra::ImageIndex(VTNodeDetalhe *nodeDet)
{
	int index = -1;

	try
	{
		//protecao
		if (nodeDet == NULL)
		{
			return index;
		}
		switch (nodeDet->Estado)
		{
			case eND_CONFIGURADO:
				index = 1;
				break;
			case eND_PENDENTE:
				index = 2;
				break;
			case eND_NAOCONTABILIZADO:
				index = 4;
				break;
			default:
				;
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return index;
}
//---------------------------------------------------------------------------
VTNodeDetalhe* __fastcall TFormFinalizaObra::NodeDetalheSelecionado(void)
{
	VTNodeDetalhe *nodeDet = NULL;

	try
	{
		if (TreeViewItems->Selected == NULL)
			{return nodeDet;}
		//verifica se o item está habilitado
		nodeDet = (VTNodeDetalhe*)TreeViewItems->Selected->Data;
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return nodeDet;
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::PanelComumAjuste(void)
{
	int height = 0;

	try
	{
		//redimensiona
		height += Height(GBoxQualidade);
		height += Height(gboxPNT);
		height += Height(GBoxNomeItem);

		PanelComum->Height = height;
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::PM_GetARIES(void)
{
	return(cfg.aries);
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::PM_GetEmpresa(void)
{
	return(cfg.empresa);
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::PM_GetMontantePNT(void)
{
	return(cfg.pnt);
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::PM_SetARIES(bool showAries)
{
	try
	{
		PanelAriesDir->Visible = showAries;
		PanelAriesEsq->Visible = showAries;

		this->ClientHeight = FormHeight();
		this->Refresh();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::PM_SetEmpresa(bool showEmpresa)
{
	try
	{
		PanelEmpresaDir->Visible = showEmpresa;
		PanelEmpresaEsq->Visible = showEmpresa;

		this->ClientHeight = FormHeight();
		this->Refresh();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::PM_SetMontantePNT(bool showPnt)
{
	try
	{
		gboxPNT->Visible = showPnt;
		//redimensiona
		CheckBoxQualidadeClick(NULL);
		PanelComumAjuste();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::PreencheInfoItemObra(VTNodeDetalhe *nodeDet)
{
//	VTItemObra *itemObra;
	bool sucesso = true;

	try
	{
		if (nodeDet == NULL)
			{sucesso = false;}
//		itemObra = nodeDet->ItemObra;
		if (itemObra_ref == NULL)
			{sucesso =  false;}

		if (sucesso == true)
		{
            EnableDetalheItem(!itemObra_ref->Configurado);
			//debug: id
	//		EditNomeItem->Text = nodeDet->Codigo;
			EditNomeItem->Text = itemObra_ref->Nome;
			//infos comuns
			//AlteraTaxaFalha
			CheckBoxQualidade->Checked = itemObra_ref->AlteraTaxaFalha;
			CheckBoxQualidadeClick(NULL);
			//TaxaFalhaInsercao
			EditTaxaInsercao->Text = DoubleToStr("%4.3f",itemObra_ref->TaxaFalhaInsercao);
			//TaxaFalhaSubstituicao
			EditTaxaSubstituicao->Text = DoubleToStr("%4.3f",itemObra_ref->TaxaFalhaSubstituicao);
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}
    //altera o botao de excluir caso ja seja excluido
	TrocaEstadoForm();
	return sucesso;
}
//---------------------------------------------------------------------------
bool  __fastcall TFormFinalizaObra::PreencheInfoObra(VTObra *obra)
{
	bool sucesso = true;

	try
	{
		//nome Obra
		EditNome->Text = obra->Nome;
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

    return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::RemoveCaptionsPanels(void)
{
	PanelComum->Caption = "";
	PanelAriesDir->Caption = "";
	PanelAriesEsq->Caption = "";
	PanelEmpresaDir->Caption = "";
    PanelEmpresaEsq->Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::SelecionaNodeRaiz(void)
{
	TTreeNode *nodeRaiz;
	VTNodeDetalhe *nodeDet = NULL;

	try
	{
		nodeRaiz = TreeViewItems->Items->GetFirstNode();

		nodeDet = (VTNodeDetalhe*) nodeRaiz->Data;

		TreeViewItems->Select(nodeRaiz);

		AtualizaItemObraRef(nodeDet);
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::TreeViewInicia(void)   //teste
{
    VTArvore    *arvore;
	VTNodeDetalhe *nodeDet;
	TTreeNode *node;

	try
	{
		arvore = multiObra_ref->Arvore;
		//TESTE PREENCHE TUDO
		//arvore->TreeViewInicia(TreeViewItems);
		TreeViewItems->Items->Clear();
		TreeViewItems->Items->BeginUpdate();
		//insere os nodes raiz
		for (int nn = 0; nn < arvore->LisNodeRaiz()->Count; nn++)
		{
			nodeDet = (VTNodeDetalhe*)arvore->LisNodeRaiz()->Items[nn];
			if (nodeDet->Habilitado == false)
				{continue;}
	//		node = TreeViewItems->Items->AddObject(NULL, nodeDet->Codigo, nodeDet);
			//insere descendentes
			node = TreeViewInsereNode(NULL, nodeDet);
		}
		TreeViewItems->FullExpand();
		TreeViewItems->Items->EndUpdate();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}


}
//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormFinalizaObra::TreeViewInsereNode(TTreeNode *pai, VTNode *node)
{
	TTreeNode *nodeAux, *treeNode;
	VTNodeDetalhe *nodeDetalhe;
	nodeAux = NULL;

	try
	{
		nodeDetalhe = (VTNodeDetalhe*) node;

		//protecao
		if (node == NULL)
		{
			return nodeAux;
		}
		//insere o node
		nodeAux = TreeViewItems->Items->AddChildObject(pai, node->Codigo, node);
		nodeAux->ImageIndex = ImageIndex(nodeDetalhe);
		nodeAux->SelectedIndex = nodeAux->ImageIndex;
		//percorre os filhos e adiciona
		for (int nn = 0; nn < node->LisNode()->Count; nn++)
		{
			nodeDetalhe = (VTNodeDetalhe*)node->LisNode()->Items[nn];
			//se estiver habilitado insere
			if (nodeDetalhe->Habilitado == false)
				{continue;}
			treeNode = TreeViewInsereNode(nodeAux, nodeDetalhe);
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
	return nodeAux;
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::TreeViewItemsAdvancedCustomDrawItem(TCustomTreeView *Sender,
		  TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
		  bool &PaintImages, bool &DefaultDraw)
{
	VTItemObra *itemObra;
	VTNodeDetalhe *nodeDet;

	try
	{
		nodeDet = (VTNodeDetalhe*) Node->Data;
		if (nodeDet == NULL)
			{return;}
		itemObra = nodeDet->ItemObra;
		if (itemObra == NULL)
			{return;}
		if(!itemObra->Contabilizar)
		{
			TreeViewItems->Canvas->Font->Style= TFontStyles() << fsStrikeOut;
		}
		else
		{
			TreeViewItems->Canvas->Font->Style= TFontStyles() >> fsStrikeOut;
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObra::TreeViewItemsClick(TObject *Sender)
{
	VTNodeDetalhe *nodeDet = NULL;

	try
	{
		nodeDet = NodeDetalheSelecionado();

		AtualizaItemObraRef(nodeDet);
		PreencheInfoItemObra(nodeDet);
		TrocaEstadoForm();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObra::TrocaEstadoForm(void)
{
	VTNodeDetalhe* nodeDet;
	bool podeExcluir;

	try
	{
		nodeDet = NodeDetalheSelecionado();

		if (nodeDet != NULL)
		{
			switch (nodeDet->Estado)
			{
				case eND_NAOCONTABILIZADO:
					podeExcluir = true;
					break;
				case eND_CONFIGURADO:
				case eND_PENDENTE:
				default:
					podeExcluir = false;
					;
			}
			if (!podeExcluir)
			{
				ToolButtonExcluir->Caption = "Excluir Item";
				ToolButtonExcluir->ImageIndex = 10;
				ToolButtonConfirma->Enabled = true;
				//desabilita o detalhar + caso esteja no ultimo item
				if ((nodeDet != NULL) && (nodeDet->Level == 4))
				{
					ToolButtonDetMais->Enabled = false;
				}
				else
				{
					ToolButtonDetMais->Enabled = true;
				}
				//verifica se pode contrair
				if ((nodeDet != NULL) &&(nodeDet->Level == 0))
				{
					ToolButtonDetMenos->Enabled = false;
				}
				else
				{
					ToolButtonDetMenos->Enabled = true;
				}
			}
			else
			{
				ToolButtonExcluir->Caption = "Incluir Item";
				ToolButtonExcluir->ImageIndex = 11;
				ToolButtonConfirma->Enabled = false;
				ToolButtonDetMenos->Enabled = false;
				ToolButtonDetMais->Enabled = false;
			}
			//o botao habiltare edicao deve aparer quando
			//esta finalizado e contabilizado

			switch (nodeDet->Estado)
			{
				case eND_CONFIGURADO:
					butHabilitaEdt->Visible = true;
					EnableDetalheItem(false);
					break;
				case eND_NAOCONTABILIZADO:
					butHabilitaEdt->Visible = false;
					EnableDetalheItem(false);
					break;
				case eND_PENDENTE:
					butHabilitaEdt->Visible = false;
					EnableDetalheItem(true);
					break;
				default:
					butHabilitaEdt->Visible = false;
					EnableDetalheItem(true);
					;
			}
			GroupBoxDetalhes->Visible = (itemObra_ref != NULL);
			//pinta o panel
			PanelDir->Color = CorPanelDir(nodeDet);
		}
		else
		{
			GroupBoxDetalhes->Visible = false;
			PanelDir->Color = clBtnFace;
		}
		if (GroupBoxDetalhes->Visible == false)
		{
			ToolButtonDetMais->Enabled = false;
			ToolButtonDetMenos->Enabled = false;
		}
		this->Refresh();
		CheckBoxQualidade->Refresh();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ValidaDouble(AnsiString string, double &valor)
{

	bool valido = true;

	try
	{
		valor = 0.;
		valido = StrToDouble(string, valor);
	}
	catch (...)
	{
		valido = false;
	}

	return valido;
}

//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ValidaInputItemObra(void)
{
	bool valido = true;
	double valor = 0;

	try
	{
		if (itemObra_ref == NULL)
			{return false;}
		//valida nome
		if (!ValidaNome(EditNomeItem->Text))
			{return false;}
		//AlteraTaxaFalha
		// é um checkbox...
		//TaxaFalhaInsercao
		if (!ValidaTaxaInsercao(valor))
			{return false;}
		//TaxaFalhaSubstituicao
		if (!ValidaTaxaSubstituicao(valor))
			{return false;}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ValidaInputObra(void)
{
	bool valido = true;
	VTItemObra *itemObra;

	try
	{//verifica se o nome
		if (obra_ref)
		{//verifica se todos itemObra estão finalizados
			if (obra_ref->LisItemObra)
			{
				for (int ni = 0; ni < obra_ref->LisItemObra->Count; ni++)
				{
					itemObra = (VTItemObra*)obra_ref->LisItemObra->Items[ni];
					if (itemObra->Configurado == false)
					{   //existe item nao finalizado
						valido = false;
						Aviso("Existem items não finalizados.");
						break;
					}
				}
			}
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ValidaNome(AnsiString nomeItem)
{

	//verifica se está vazio
	if (nomeItem.IsEmpty() == true)
	{
		Aviso("O nome do item não pode ser vazio");
		return false;
	}
	//verifica se possui mais de 200 caracteres
    else if (nomeItem.Length() > 200)
	{
		Aviso("O nome do item não ter mais que 200 caracteres");
		return false;
	}
    //troca caracteres invalidos
	nomeItem = ASCIIFilterWithAccents(nomeItem);

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObra::ValidaPNT(double &valor)
{
	bool valido = false;

	valido = ValidaDouble(EditPNT->Text,valor);

	return valido;
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16149781
bool __fastcall TFormFinalizaObra::ValidaTaxaInsercao(double &valor)
{
	bool valido = false;

	valido = ValidaDouble(EditTaxaInsercao->Text,valor);

	if(! valido)
	{
		Aviso("Redução da taxa de falha devido à inserção de equipamento inválida");
		valor = 0.;
	}
	return valido;
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15755250
//https://sinapsisenergia.teamwork.com/#tasks/16149781
bool __fastcall TFormFinalizaObra::ValidaTaxaSubstituicao(double &valor)
{
	bool valido = false;

	valido = ValidaDouble(EditTaxaSubstituicao->Text,valor);
    if(! valido)
	{
		Aviso("Redução da taxa de falha devido à substituição de equipamento inválida");
		valor = 0.;
	}

	if (valor > 100)
	{
		valido = false;
		Aviso("A redução da taxa de falha devido à substituição de equipamento não pode ser superior à 100%.");
        EditTaxaSubstituicao->Text = "100";
	}

	return valido;
}
//---------------------------------------------------------------------------














void __fastcall TFormFinalizaObra::TreeViewItemsExpanded(TObject *Sender, TTreeNode *Node)

{
    //teste
	int a = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObra::TreeViewItemsCollapsed(TObject *Sender, TTreeNode *Node)

{
	//teste
	int a = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObra::TreeViewItemsDeletion(TObject *Sender, TTreeNode *Node)

{
    //teste
	int a = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObra::TreeViewItemsCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw)

{
	VTNodeDetalhe *nodeDet;

	//teste
	nodeDet = (VTNodeDetalhe*)Node->Data;
	if (nodeDet != NULL)
	{
		Node->ImageIndex = ImageIndex(nodeDet);
		Node->SelectedIndex = Node->ImageIndex;
	}
}
//---------------------------------------------------------------------------


