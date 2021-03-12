// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <syscurr.h>
#include <System.SysUtils.hpp>
#include "TFormFinalizaObraARIES.h"
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\MultiObra\VTMultiObra.h>
#include <PlataformaSinap\Fontes\MultiObra\VTNodeDetalhe.h>
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\ItemObra\VTItemObraARIES.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
// ---------------------------------------------------------------------------
#include <ModulosOpcionais\Fontes\BaseGRP\VTIpeGRP.h>
#include <ModulosOpcionais\Fontes\BaseGRP\VTPadrao.h>
// ---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormFinalizaObra"
#pragma resource "*.dfm"
TFormFinalizaObraARIES *FormFinalizaObraARIES;

// ---------------------------------------------------------------------------
__fastcall TFormFinalizaObraARIES::TFormFinalizaObraARIES(TComponent* Owner, VTApl *apl_owner,
	VTObra *obra) : TFormFinalizaObra(Owner, apl_owner, obra)
{
    AtualizaCustosTotal();
}

// ---------------------------------------------------------------------------
__fastcall TFormFinalizaObraARIES::~TFormFinalizaObraARIES(void)
{

}

// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::AtualizaCustosItem(void)
{
	VTItemObraARIES *item;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		if (item != NULL)
		{
			item->ContabilizaCusto();
			EditCustoItem->Text = FormataValorMonetario(item->Custo);
		}
		// atualiza os custos totais
		AtualizaCustosTotal();
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
void __fastcall TFormFinalizaObraARIES::AtualizaCustosTotal(void)
{
	double valor = 0;
	bool possuiPendente = false;
	VTItemObraARIES *item, *itemAux;

	try
	{
		// os custos totais é a soma dos custos dos items de todos items finalizados
		if (obra_ref != NULL)
		{
			for (int ni = 0; ni < obra_ref->LisItemObra->Count; ni++)
			{
				item = (VTItemObraARIES*)obra_ref->LisItemObra->Items[ni];
				// verifica se foi configurado
				if (!item->Configurado)
				{
					possuiPendente = true;
				}
				else
				{
					if (item->Contabilizar)
					{
						valor += item->Custo;
					}
				}
			}
		}
		EditCusto->Text = FormataValorMonetario(valor);
		// pinta o edt de cor diferente caso esteja finalizado
		if (possuiPendente == true)
		{
			EditCusto->Color = clWebCornSilk;
		}
		else
		{
			EditCusto->Color = clWebHoneydew;
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
void __fastcall TFormFinalizaObraARIES::AtualizaQuantidade(TEdit *edit)
{
	VTItemObraARIES *item;

	try
	{
		if (itemObra_ref != NULL)
		{
			item = (VTItemObraARIES*) itemObra_ref;

			if (edit == EditQntInsercao)
			{
				EditQntInsercao->Text = DoubleToStr("%4.3f", item->QuantidadeIns);
			}
			else if(edit == EditQntRetirada)
			{
				EditQntRetirada->Text = DoubleToStr("%4.3f", item->QuantidadeIns);
			}
			else
			{
				//faz nada
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
// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::CBoxPadraoInsChange(TObject *Sender)
{
	VTItemObraARIES *item;
	int index = -1;
	VTPadrao *padraoNovo;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		// atualiza
		if (item != NULL)
		{
			index = CBoxPadraoIns->ItemIndex;
			if (index >= 0)
			{
				padraoNovo = (VTPadrao*)CBoxPadraoIns->Items->Objects[index];
				MudaPadraoConstrutivo(item->PadraoIns, padraoNovo, true);
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

// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::CBoxPadraoRetChange(TObject *Sender)
{
	VTItemObraARIES *item;
	int index = -1;
	VTPadrao *padraoNovo;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		// atualiza
		if (item != NULL)
		{
			index = CBoxPadraoRet->ItemIndex;
			if (index >= 0)
			{
				padraoNovo = (VTPadrao*)CBoxPadraoRet->Items->Objects[index];
				MudaPadraoConstrutivo(item->PadraoRet, padraoNovo, false);
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
void __fastcall TFormFinalizaObraARIES::ConfirmaItemProprio(void)
{
	VTItemObraARIES *item;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		//quantidades insercao
		item->QuantidadeIns = StrToDouble(EditQntInsercao->Text);
		//quantidades retirada
		item->QuantidadeRet = StrToDouble(EditQntRetirada->Text);
		//cofirma infos basicas
		TFormFinalizaObra::ConfirmaItemProprio();
		 //atualiza custo total
		AtualizaCustosTotal();
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
bool __fastcall TFormFinalizaObraARIES::ConfirmaObraPropria(void)
{
	bool sucesso = true;
	try
	{
		sucesso = TFormFinalizaObra::ConfirmaObraPropria();
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
// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::EscolhePadraoConstrutivo(TComboBox *combo, VTPadrao *padrao)
{
	int id;

	try
	{
		if (padrao == NULL)
		{
			return;
		}
		if (combo == NULL)
		{
			return;
		}
		id = padrao->Id;

		if (combo->Items->Count > 0)
		{
			combo->ItemIndex = 0;
			// percorre todos items
			for (int ni = 0; ni < combo->Items->Count; ni++)
			{
				padrao = (VTPadrao*) combo->Items->Objects[ni];
				if ((padrao != NULL) && (padrao->Id == id))
				{
					combo->ItemIndex = ni;
					break;
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
}

// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::EditQntInsercaoExit(TObject *Sender)
{
	VTItemObraARIES *item;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		if (!ValidaQuantidade(EditQntInsercao))
		{
			Aviso("Valor inválido.");
		}
		else
		{ // atualiza
			if (item)
			{
				item->QuantidadeIns = StrToDouble(EditQntInsercao->Text);
				AtualizaCustosItem();
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
// ---------------------------------------------------------------------------

void __fastcall TFormFinalizaObraARIES::EditQntRetiradaExit(TObject *Sender)
{
    VTItemObraARIES *item;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		if (!ValidaQuantidade(EditQntRetirada))
		{
			Aviso("Valor inválido.");
		}
		else
		{ // atualiza
			if (item)
			{
				item->QuantidadeRet = StrToDouble(EditQntRetirada->Text);
//				AtualizaCustosItem();
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
void   __fastcall TFormFinalizaObraARIES::EnableDetalheItem(bool enable)
{
	try
	{
		TFormFinalizaObra::EnableDetalheItem(enable);

		EditCustoItem->Enabled = enable;
		CBoxPadraoIns->Enabled = enable;
		EditQntInsercao->Enabled = enable;
		CBoxPadraoRet->Enabled = enable;
		EditQntRetirada->Enabled = enable;
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
AnsiString __fastcall TFormFinalizaObraARIES::FormataValorMonetario(double valor)
{
	AnsiString custoStr = "-";
	// Currency valorCurr;
	TFormatSettings format = TFormatSettings::Create(GetThreadLocale());

	// valorCurr = valor;
	try
	{
		custoStr = CurrToStrF(valor, ffCurrency, 2, format);
	}
	catch (...)
	{
		custoStr = "-";
	}

	return custoStr;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16917117
void __fastcall TFormFinalizaObraARIES::MudaPadraoConstrutivo(VTPadrao *antes, VTPadrao *depois, bool insercao)
{

    VTItemObraARIES *item;
	double valor = 0;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;
		if (item != NULL)
		{
			if (insercao)
			{
				item->PadraoIns = depois;
				// e por isso atualiza os custos
				AtualizaCustosItem();
				//atualiza a visualizacao
				AtualizaQuantidade(EditQntInsercao);
				//verifica se a quantidade é nula
				try
				{
					valor = StrToDouble(EditQntInsercao->Text);
				}
				catch(...)
				{
					valor = 0;
				}
				if (IsDoubleZero(valor) && (depois != NULL))
				{
					Aviso("Padrão construtivo não corresponde às ações de edição da rede");
				}
				//acerta o caption
				if ((depois != NULL) &&(depois->TipoEqpto == eqptoTRECHO))
				{
					LabelQntInsercao->Caption = "Quantidade [km]: ";
				}
				else
				{
					LabelQntInsercao->Caption = "Quantidade [un]: ";
				}
			}
			else
			{
				item->PadraoRet = depois;
				//atualiza a visualizacao
				AtualizaQuantidade(EditQntRetirada);
				//verifica se a quantidade é nula
				try
				{
					valor = StrToDouble(EditQntRetirada->Text);
				}
				catch(...)
				{
					valor = 0;
				}
				if (IsDoubleZero(valor) && (depois != NULL))
				{
					Aviso("Padrão construtivo não corresponde às ações de edição da rede");
				}
				//acerta o caption
				if ((depois != NULL) &&(depois->TipoEqpto == eqptoTRECHO))
				{
					LabelQntInsercao->Caption = "Quantidade [km]: ";
				}
				else
				{
					LabelQntInsercao->Caption = "Quantidade [un]: ";
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
}
//// ---------------------------------------------------------------------------
//bool __fastcall TFormFinalizaObraARIES::PadraoesTemTipoEqptoIguais(VTPadrao *padrao1,
//	VTPadrao *padrao2)
//{
//	if (padrao1 == padrao2)
//	{
//		return true;
//	}
//	if (padrao1 == NULL)
//	{
//		return false;
//	}
//	if (padrao2 == NULL)
//	{
//		return false;
//	}
//	if (padrao1->TipoEqpto != padrao2->TipoEqpto)
//	{
//		return false;
//	}
//
//	return true;
//}

// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraARIES::PreencheCombosPadrao(VTNodeDetalhe *nodeDet)
{
	// variáveis locais
	VTPadrao *padrao;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp;
	TList *lisPADRAO;

	try
	{
		ipe_grp = (VTIpeGRP*)planejamento->Ipe;
		lisPADRAO = ipe_grp->LisPadrao();

		// reinicia CBoxPadrao
		CBoxPadraoIns->Clear();
		CBoxPadraoRet->Clear();
		CBoxPadraoIns->Items->BeginUpdate();
		CBoxPadraoRet->Items->BeginUpdate();
		// insere os vazios
		CBoxPadraoIns->Items->AddObject("", NULL);
		CBoxPadraoRet->Items->AddObject("", NULL);
		for (int n = 0; n < lisPADRAO->Count; n++)
		{
			padrao = (VTPadrao*)lisPADRAO->Items[n];
			// adiciona dependendo do tipo
			if ((nodeDet != NULL) && (nodeDet->TipoEqpto != eqptoINDEF))
			{
				if (padrao->TipoEqpto == nodeDet->TipoEqpto)
				{
					CBoxPadraoIns->Items->AddObject(padrao->Descricao, padrao);
					CBoxPadraoRet->Items->AddObject(padrao->Descricao, padrao);
				}
			}
			else
			{
				CBoxPadraoIns->Items->AddObject(padrao->Descricao, padrao);
				CBoxPadraoRet->Items->AddObject(padrao->Descricao, padrao);
			}
		}
		// seleciona o primeiro
		if (CBoxPadraoIns->Items->Count > 0)
			CBoxPadraoIns->ItemIndex = 0;
		if (CBoxPadraoRet->Items->Count > 0)
			CBoxPadraoRet->ItemIndex = 0;
		CBoxPadraoIns->Sorted = true;
		CBoxPadraoRet->Sorted = true;
		CBoxPadraoIns->Items->EndUpdate();
		CBoxPadraoRet->Items->EndUpdate();
		//verifica se nao possui padrao construtivo disponivel
		if ( (CBoxPadraoIns->Items->Count <= 1) && (CBoxPadraoRet->Items->Count <= 1))
		{
			if ((itemObra_ref != NULL)&&(itemObra_ref->Contabilizar))
				{ActionEdtExcluir->Execute();}
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
bool __fastcall TFormFinalizaObraARIES::PreencheInfoItemObra(VTNodeDetalhe *nodeDet)
{
	bool sucesso = true;
	VTItemObraARIES *item;

	try
	{
		PreencheCombosPadrao(nodeDet);
		try
		{
			if ((sucesso = TFormFinalizaObra::PreencheInfoItemObra(nodeDet)) == false)
				{return false;}
			if (itemObra_ref != NULL)
			{
				item = (VTItemObraARIES*)itemObra_ref;
				//preenche padrao construtivo de insercao
				EscolhePadraoConstrutivo(CBoxPadraoIns, item->PadraoIns);
				//preenche quantidade de insercao
				EditQntInsercao->Text = DoubleToStr("%4.3f", item->QuantidadeIns);
				//preenche padrao construtivo de retirada
				EscolhePadraoConstrutivo(CBoxPadraoRet, item->PadraoRet);
				//preenche quantidade de retirada
				EditQntRetirada->Text = DoubleToStr("%4.3f", item->QuantidadeRet);
				//preenche o custo
				EditCustoItem->Text = FormataValorMonetario(item->Custo);
			}

		}
		catch (...)
		{
			sucesso = false;
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

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::PreencheInfoObra(VTObra * obra)
{
	bool sucesso = true;
	try
	{
		if ((sucesso = TFormFinalizaObra::PreencheInfoObra(obra)) == false)
			{return false;}
		//preenche o custo total
        AtualizaCustosTotal();
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaInputObra(void)
{
	bool sucesso = true;

	try
	{
		// faz as validacoes basicas
		sucesso = TFormFinalizaObra::ValidaInputObra();
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

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaDouble(AnsiString string)
{
	double valor;
	bool valido = true;

	try
	{
		valor = 0.;
		valor = StrToDouble(string);
		valido = !IsDoubleZero(valor, 1e-4);
	}
	catch (...)
	{
		valido = false;
	}

	return valido;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaInputItemObra(void)
{
	bool sucesso = true;

	try
	{
		// faz as validacoes basicas
		sucesso = TFormFinalizaObra::ValidaInputItemObra();

		if (sucesso)
		{
			try
			{ // valida os padroes constrituvos
				if ((sucesso = ValidaPadroesConstrutivos()) == false)
				{
					Aviso("Selecione pelo menos um padrão construtivo");
					return false;
				}
				// valida as quantidades
				if ((sucesso = ValidaQuantidades()) == false)
				{
					return false;
				}
			}
			catch (...)
			{
				sucesso = false;
			}
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

	return sucesso;
}

//// ---------------------------------------------------------------------------
//bool __fastcall TFormFinalizaObraARIES::ValidaPadroesConstrutivos(void)
//{
//   	VTItemObraARIES *item;
//	bool sucesso = true;
//
//	try
//	{
//		item = (VTItemObraARIES*) itemObra_ref;
//
//		// verifica se foi selecionado um padrão construtivo
//		if (CBoxPadraoIns->ItemIndex <= 0)
//		{
//			return false;
//		}
//		// caso nao possua padrao de retirada, poe igual de insercao
//		if (CBoxPadraoRet->ItemIndex <= 0)
//		{
//	//		CBoxPadraoRet->ItemIndex = CBoxPadraoIns->ItemIndex;
//			MudaPadraoConstrutivo(item->PadraoRet,item->PadraoIns, false);
//		}
//	}
//    catch(Exception &e)
//	{
//        AnsiString funcao = __PRETTY_FUNCTION__;
//		Erro(e.Message + " em " + funcao);
//		sucesso = false;
//	}
//	catch(...)
//	{
//		 AnsiString funcao = __PRETTY_FUNCTION__;
//		 Erro(funcao);
//		 sucesso = false;
//	}
//
//	return sucesso;
//}// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaPadroesConstrutivos(void)
{
   	VTItemObraARIES *item;
	bool sucesso = false;

	try
	{
		item = (VTItemObraARIES*) itemObra_ref;

		// verifica se foi selecionado um padrão construtivo
		if (CBoxPadraoIns->ItemIndex > 0)
		{
			sucesso |= true;
		}
		// verifica se foi selecionado um padrão construtivo
		if (CBoxPadraoRet->ItemIndex > 0)
		{
            sucesso |= true;
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

	return sucesso;
}
// ---------------------------------------------------------------------------
// bool __fastcall TFormFinalizaObraARIES::ValidaPadraoConstrutivo(TComboBox *combo)
// {
//
// }

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaQuantidade(TEdit * edt)
{
	AnsiString valor;
	bool sucesso = false;

	try
	{
		valor = edt->Text;
		sucesso = ValidaDouble(valor);
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

// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraARIES::ValidaQuantidades(void)
{
	bool sucesso = true;
	bool sucessoIns = true;
	bool sucessoRet = true;

	try
	{
		// caso possua padrao construtivo de insercao verifica o edt
		if (CBoxPadraoIns->ItemIndex > 0)
		{
			sucessoIns = ValidaQuantidade(EditQntInsercao);
			sucesso &= sucessoIns;
			if (sucessoIns == false)
			{
				Aviso("Quantidade de inserção inválida.");
			}
		}
		if (CBoxPadraoRet->ItemIndex > 0)
		{
			sucessoRet = ValidaQuantidade(EditQntRetirada);
			sucesso &= sucessoRet;
			if (sucessoRet == false)
			{
				Aviso("Quantidade de retirada inválida.");
			}
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

	return sucesso;
}
// ---------------------------------------------------------------------------


