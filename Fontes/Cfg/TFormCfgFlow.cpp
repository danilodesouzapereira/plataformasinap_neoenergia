// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFormCfgFlow.h"
#include "TFormCfgLimV.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BD\Rede\VTBDGeral.h"
#include "..\Diretorio\DirArq.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTPadraoRede.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Grafico\VTUnif.h"
#include "..\Rede\VTTipoRede.h"

#include "..\Sinap\TFormMDI.h"
#include "..\Sinap\TFormChild.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Geral.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCfgFlow::TFormCfgFlow(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	AnsiString class_name;
	TFormChild *FormChild;
//	VTGeral *geralSel;
//	VTApl *apl_child;
	// salva ponteiro p/ objeto
	apl = apl_owner;
	geral_mdi = (VTGeral*)apl->GetObject(__classid(VTGeral));

	geral_RedeEmpresa = NULL;

	lisFormChild = new TList;

	formMDI = NULL;
	//teste
	if (Owner != NULL)
	{
		//verifica se o owner é um formMDI
		class_name = Owner->ClassName();
		if (class_name.AnsiCompareIC("TFormMDI") == 0)
		{
			formMDI = (TFormMDI*)Owner;
            //percorre todos os filhos em busca dos FormChilds
			for (int n = 0; n < formMDI->MDIChildCount; n++)
			{
				class_name = formMDI->MDIChildren[n]->ClassName();
				if (class_name.AnsiCompareIC("TFormChild") != 0)
					continue;
				FormChild = (TFormChild*)formMDI->MDIChildren[n];
				//obtem o geral do formChild
				lisFormChild->Add(FormChild);
			}
		}
	}
	//inicia o combo box da base selecionada
	CBoxBaseSelecionadaInicia();
	//emula o comando de mudar o combobox
    cboxBaseSelecionada->OnChange(NULL);
//	//verifica qual foi selecionado
//	formChildEscolhido = FormChildSelecionado();
//	try
//	{   //verifica se foi a opcao que nao tem formchild aberto
//		if (formChildEscolhido == NULL)
//		{
//			geral_RedeEmpresa = DLL_NewObjGeral(apl);
//			// inicia objeto Geral local com objeto geral_mdi
//			geral_RedeEmpresa->CopiaAtributosDe(*geral_mdi);
//			//le os valores da base rede empresa
//			geral_RedeEmpresa->LeGeral(true);
//			//seleciona o geral
//			geralSel = geral_RedeEmpresa;
//
//		}
//		else
//		{
//			//obtem o geral do apl do form child
//			apl_child = formChildEscolhido->Apl;
//			if (apl_child != NULL)
//			{
//				geralSel = (VTGeral*)apl_child->GetObject(__classid(VTGeral));
//			}
//		}
//		if (geralSel == NULL)
//		{
//			geralSel = geral_mdi;
//		}
//		//cria um geral provisorio para guardar as alteracoes e o exibe
//		ExibeDados(geralSel);
//	}
//	catch (Exception &e)
//	{
//	}
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgFlow::~TFormCfgFlow(void)
{
	// destrói objetos criados
	if (geralTemp)
	{
		delete geralTemp;
		geralTemp = NULL;
	}
	if (geral_RedeEmpresa)
	{
		delete geral_RedeEmpresa;
		geral_RedeEmpresa = NULL;
	}
	if (lisFormChild)
	{
		delete lisFormChild;
		lisFormChild = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionConfirmaExecute(TObject *Sender)
{
	TFormChild *formChild, *form;
	VTApl *apl_selecionado;
	VTApl *apl_aux;
	VTGeral *geralAux;
	VTFlow *flow;

	// lê e valida dados editados
	if (!LeDados())
		return;

	//Aplica as novas configs ao formChild
	if ((formChild = FormChildSelecionado())!= NULL)
	{
        //recupera o apl do selecionado
		apl_selecionado = formChild->Apl;
		if((lisFormChild->Count > 1) && (Confirma("Deseja aplicar os mesmos limites à todas bases abertas?",
			 "Em caso negativo aplica-se somente à base selecionada.") == idYes))
		{
			//faz primeiro o da base selecionada q vai virar padrao pras outras
			apl_aux = formChild->Apl;
			geralAux = (VTGeral*)apl_aux->GetObject(__classid(VTGeral));
			//copia no geral do form o geral temporario
			geralAux->CopiaAtributosDe(*geralTemp,true);
			//atualiza o geral
			SalvaValoresTemp(geralAux);
			// atualiza base de dados
			BaseDadoSalvaGeral(geralAux);
			//configura o apl selecionado
			ConfiguraGeral(apl_selecionado,apl_aux);
			for (int i = 0; i < lisFormChild->Count; i++)
			{
				form = (TFormChild*)lisFormChild->Items[i];
				if(form == formChild)
                    {continue;}
				apl_aux = form->Apl;
				geralAux = (VTGeral*)apl_aux->GetObject(__classid(VTGeral));
				if (geralAux != NULL)
				{
					//verifica se os valores foram alterados
					if (geralAux->PadroesRedesDiferentes(*geralTemp))
					{
						flow = (VTFlow*)apl_aux->GetObject(__classid(VTFlow));
						if (flow != NULL)
						{
							flow->UpToDate = false;
						}
					}
					//copia no geral do form o geral temporario
					geralAux->CopiaAtributosDe(*geralTemp,true);
					//atualiza o geral
					SalvaValoresTemp(geralAux);
					// atualiza base de dados
					BaseDadoSalvaGeral(geralAux);
					//configura o apl selecionado
					ConfiguraGeral(apl_selecionado,apl_aux);
				}
			}
		}
		else
		{
			apl_aux = formChild->Apl;
			geralAux = (VTGeral*)apl_aux->GetObject(__classid(VTGeral));
            //verifica se os valores foram alterados
			if (geralAux->PadroesRedesDiferentes(*geralTemp))
			{
				flow = (VTFlow*)apl_aux->GetObject(__classid(VTFlow));
				if (flow != NULL)
				{
					flow->UpToDate = false;
				}
			}
			//copia no geral do form o geral temporario
			geralAux->CopiaAtributosDe(*geralTemp,true);
			//atualiza o geral
			SalvaValoresTemp(geralAux);
			// atualiza base de dados
			BaseDadoSalvaGeral(geralAux);
			//configura o apl selecionado
			ConfiguraGeral(apl_aux, apl_aux);
		}
	}
	else
	{
		//copia no geral do form o geral temporario
		geral_RedeEmpresa->CopiaAtributosDe(*geralTemp,true);
		//atualiza o geral
		SalvaValoresTemp(geral_RedeEmpresa);
		// atualiza base de dados
		BaseDadoSalvaGeral(geral_RedeEmpresa);
		// atualiza objeto geral_mdi
//		geral_mdi->CopiaAtributosDe(*geral_RedeEmpresa,true);
    }

	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionCfgLimVExecute(TObject *Sender)
{
	TForm *form;

	form = new TFormCfgLimV(this,apl);

	form->ShowModal();

	delete form;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionDefaultExecute(TObject *Sender)
{
	// reinicia c/ valores default
	// ReiniciaValoresDefault();
	geralTemp->ReiniciaValoresDefault();
	//carrega novamente os valores para o temp
	CarregaValoresTemp();
	// atualiza tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionExibeLblExecute(TObject *Sender)
{
	// atualiza labels de tensão
//	lblV1->Caption = "abaixo de " + edtV3->Text;
	lblV2->Caption = "entre " + edtV3->Text + " e ";// + edtV1->Text;
	lblV3->Caption = "entre " + edtV1->Text + " e ";// + edtV2->Text;
	lblVlimsup->Caption = edtV4->Text;
	lblV4->Caption = "entre " + edtV2->Text + " e ";// + edtV4->Text;
//	lblV5->Caption = "acima de " + edtV4->Text;
	// atualiza labels de corrente
	//lblI1->Caption = "abaixo de " + edtI1->Text;
	lblI2->Caption = "entre " + edtI1->Text + " e ";// + edtI2->Text;
	lblClimsup->Caption = edtI2->Text;
	//lblI3->Caption = "acima de " + edtI2->Text;
	// atualiza labels de potência de trado
	//lblS1->Caption = "abaixo de " + edtS1->Text;
	lblS2->Caption = "entre " + edtS1->Text + " e ";// + edtS2->Text;
	lblSlimsup->Caption = edtS2->Text;
	//lblS3->Caption = "acima de " + edtS2->Text;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::AtualizaValoresTemp(int tab)
{
	if(tab > 0)
	{
		// faixa A de Tensão
		valoresTemp[tab][1] = StrToDouble(edtV1->Text);
		valoresTemp[tab][2] = StrToDouble(edtV2->Text);
		// faixa B de Tensão
		valoresTemp[tab][0] = StrToDouble(edtV3->Text);
		valoresTemp[tab][3] = StrToDouble(edtV4->Text);
	}

}
// ---------------------------------------------------------------------------
//bool __fastcall TFormCfgFlow::BaseDadoLeGeral(void)
//{
//	// variáveis locais
//	bool sucesso = false;
//	VTBD *bd;
//
//	// OOPS: modificado em 20120611
//	if ((bd = DLL_NewObjBD(apl)) == NULL)
//		return (false);
//	// lê dados do objeto Geral
//	if (bd->Conecta())
//	{
//		sucesso = bd->Geral->LeGeral(geral);
//	}
//	// destrói objeto
//	bd->Desconecta();
//	delete bd;
//	bd = NULL;
//
//	return (sucesso);
//}

//// ---------------------------------------------------------------------------
//bool __fastcall TFormCfgFlow::BaseDadoSalvaGeral(void)
//{
//	// variáveis locais
//	bool sucesso = false;
//	VTBD *bd;
//
//	// OOPS: modificado em 20120611
//	if ((bd = DLL_NewObjBD(apl)) == NULL)
//		return (false);
//	// lê dados do objeto Geral
//	if (bd->Conecta())
//	{
//		sucesso = bd->Geral->SalvaGeral(geral);
//	}
//	// destrói objeto
//	bd->Desconecta();
//	delete bd;
//	bd = NULL;
//
//	// verifica se gravou com sucesso
//	if (sucesso)
//	{ // atualiza objeto geral_mdi
//		geral_mdi->CopiaAtributosDe(*geral);
//	}
//	return (sucesso);
//}// ---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::BaseDadoSalvaGeral(VTGeral *pGeral)
{
	// variáveis locais
	bool sucesso = false;
	bool isRedeEmpresa = false;

	// lê dados do objeto Geral
	if (geral_RedeEmpresa == pGeral)
	{
		isRedeEmpresa = true;
	}
	sucesso = pGeral->SalvaGeral(isRedeEmpresa);
//	sucesso = bd->Geral->SalvaGeral(geral);

	// verifica se gravou com sucesso
//	if (sucesso)
//	{ // atualiza objeto geral_mdi
//		geral_mdi->CopiaAtributosDe(*geral);
//	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::CarregaValoresTemp(void)
{
	VTPadraoRede *padraoRede;

	//carrega valores redeSEC
	padraoRede = geralTemp->ExistePadraoRede(redeSEC);
	if(padraoRede != NULL)
	{
		valoresTemp[tabCFGV_REDESEC][0] = padraoRede->V1;
		valoresTemp[tabCFGV_REDESEC][1] = padraoRede->V2;
		valoresTemp[tabCFGV_REDESEC][2] = padraoRede->V3;
		valoresTemp[tabCFGV_REDESEC][3] = padraoRede->V4;
	}
	//carrega valores redePRI
	padraoRede = geralTemp->ExistePadraoRede(redePRI);
	if(padraoRede != NULL)
	{
		valoresTemp[tabCFGV_REDEPRI][0] = padraoRede->V1;
		valoresTemp[tabCFGV_REDEPRI][1] = padraoRede->V2;
		valoresTemp[tabCFGV_REDEPRI][2] = padraoRede->V3;
		valoresTemp[tabCFGV_REDEPRI][3] = padraoRede->V4;
	}
	//carrega valores redeSUB
	padraoRede = geralTemp->ExistePadraoRede(redeSUB);
	if(padraoRede != NULL)
	{
		valoresTemp[tabCFGV_REDESUB][0] = padraoRede->V1;
		valoresTemp[tabCFGV_REDESUB][1] = padraoRede->V2;
		valoresTemp[tabCFGV_REDESUB][2] = padraoRede->V3;
		valoresTemp[tabCFGV_REDESUB][3] = padraoRede->V4;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormCfgFlow::cboxBaseSelecionadaChange(TObject *Sender)
{
	VTGeral *geralSel;
	VTApl *apl_child;

	//verifica qual foi selecionado
	formChildEscolhido = FormChildSelecionado();
	try
	{   //verifica se foi a opcao que nao tem formchild aberto
		if (formChildEscolhido == NULL)
		{
			if (geral_RedeEmpresa != NULL)
			{
                delete geral_RedeEmpresa;
			}
			geral_RedeEmpresa = DLL_NewObjGeral(apl);
			// inicia objeto Geral local com objeto geral_mdi
			geral_RedeEmpresa->CopiaAtributosDe(*geral_mdi);
			//le os valores da base rede empresa
			geral_RedeEmpresa->LeGeral(true);
			//seleciona o geral
			geralSel = geral_RedeEmpresa;

		}
		else
		{
			//obtem o geral do apl do form child
			apl_child = formChildEscolhido->Apl;
			if (apl_child != NULL)
			{
				geralSel = (VTGeral*)apl_child->GetObject(__classid(VTGeral));
			}
		}
		if (geralSel == NULL)
		{
			geralSel = geral_mdi;
		}
		//cria um geral provisorio para guardar as alteracoes e o exibe
		ExibeDados(geralSel);
	}
	catch (Exception &e)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::CBoxBaseSelecionadaInicia(void)
{
	TFormChild *formChild;
	AnsiString nomeBase;

	//limpa cbox
	cboxBaseSelecionada->Clear();
	cboxBaseSelecionada->Items->BeginUpdate();
	//adiciona a opção de todas as bases futuras
	cboxBaseSelecionada->AddItem("Todas futuras bases", NULL);
	//adiciona as bases abertas
	for (int n = 0; n < lisFormChild->Count; n++)
	{
		formChild  = (TFormChild*) lisFormChild->Items[n];
		nomeBase = NomeBase(formChild);
		if (!nomeBase.IsEmpty())
		{
			cboxBaseSelecionada->AddItem(nomeBase,formChild);
		}
	}
	cboxBaseSelecionada->Items->EndUpdate();
	//reeinicia a variavel formchild
	formChild = NULL;
	//busca o formchild selecionado
	if (formMDI != NULL)
	{
		formChild =(TFormChild*) formMDI->ActiveMDIChild;;
	}
    //seleciona o formchild ativo (caso null, é q nao tem rede aberta)
	SelecionaBaseFormChild(formChild);
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ConfiguraGeral(VTApl *apl_de, VTApl *apl_para)
{
    // variáveis locais
	VTGeral *geral = (VTGeral*)apl_para->GetObject(__classid(VTGeral));
	VTGeral *geral_ext = (VTGeral*)apl_de->GetObject(__classid(VTGeral));
	VTFlow *flow = (VTFlow*)apl_para->GetObject(__classid(VTFlow));
	VTGrafico *grafico = (VTGrafico*)apl_para->GetObject(__classid(VTGrafico));
	VTSinapChild *sinapChild = (VTSinapChild*)apl_para->GetObject(__classid(VTSinapChild));
	VTUnif *unif;

	//verifica a necessidade de atualizar mudar o fluxo
	if (geral->PadroesRedesDiferentes(*geral_ext))
	{// marca que o fluxo não está atualizado
		flow->UpToDate = false;
	}
	// atualiza objeto geral próprio a partir do objeto geral recebido
	geral->CopiaAtributosDe(*geral_ext);
	// verifica se está exibindo tela de fluxo
	if ((sinapChild != NULL) && (sinapChild->ButFlow->Down))
	{ // recalcula fluxo
		sinapChild->ActionFlow->Execute();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeDados(VTGeral *pGeral)
{
	bool redeEmpresa = false;

	if(pGeral == geral_RedeEmpresa)
	{
		redeEmpresa = true;
	}
	pGeral->LeGeral(redeEmpresa);
	// cria objeto Geral local
	if (geralTemp != NULL)
	{
		delete geralTemp;
	}
	geralTemp = DLL_NewObjGeral(apl);
	// inicia objeto Geral local com objeto geral selecionado
	geralTemp->CopiaAtributosDe(*pGeral);
	///carega os valores do geral pro form
	CarregaValoresTemp();
	// atualiza tela
	ExibeDados();
	//carega os valores do geral pro form
	//atualiza o valor da tab
	tabAnterior = tcFaixaV->TabIndex;
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeDados(void)
{
	// faixas de Tensão
	ExibeDadosTensao();
	// faixa de Corrente
	edtI1->Text = IntToStr((int)((geralTemp->FLOW.I[0] + .0001) * 100));
	edtI2->Text = IntToStr((int)((geralTemp->FLOW.I[1] + .0001) * 100));
	// faixa de potência
	edtS1->Text = IntToStr((int)((geralTemp->FLOW.S[0] + .0001) * 100));
	edtS2->Text = IntToStr((int)((geralTemp->FLOW.S[1] + .0001) * 100));
	// atualiza cores dos Panels
	ExibeCores();
	// atualiza labels de valores intermediários
	ActionExibeLblExecute(NULL);
}
//// ---------------------------------------------------------------------------
//void __fastcall TFormCfgFlow::ExibeDadosTensao(void)
//{
//	int tipoSegmento = -1;
//
//	tipoSegmento = TipoRedeTabTensao();
//	if(tipoSegmento >0)
//	{
//		// faixa A de Tensão
//		edtV1->Text = DoubleToStr("%5.3f", geral->FLOW.V[tipoSegmento][1]);
//		edtV2->Text = DoubleToStr("%5.3f", geral->FLOW.V[tipoSegmento][2]);
//		// faixa B de Tensão
//		edtV3->Text = DoubleToStr("%5.3f", geral->FLOW.V[tipoSegmento][0]);
//		edtV4->Text = DoubleToStr("%5.3f", geral->FLOW.V[tipoSegmento][3]);
//	}
//}// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeDadosTensao(void)
{
	int tabIndex = -1;

	tabIndex = tcFaixaV->TabIndex;
	if(tabIndex >= 0)
	{
		// faixa A de Tensão
		edtV1->Text = DoubleToStr("%5.3f", valoresTemp[tabIndex][1]);
		edtV2->Text = DoubleToStr("%5.3f", valoresTemp[tabIndex][2]);
		// faixa B de Tensão
		edtV3->Text = DoubleToStr("%5.3f", valoresTemp[tabIndex][0]);
		edtV4->Text = DoubleToStr("%5.3f", valoresTemp[tabIndex][3]);
	}
	ActionExibeLblExecute(NULL);
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeCores(void)
{
	// cores para diagnóstico
	PanelCorDiagRuim->Color = geralTemp->COR.Faixa[diagRUIM];
	PanelCorDiagRegular->Color = geralTemp->COR.Faixa[diagREGULAR];
	PanelCorDiagBom->Color = geralTemp->COR.Faixa[diagBOM];
	// cores para tensões
	panCorV1->Color = geralTemp->COR.Faixa[diagRUIM];
	panCorV2->Color = geralTemp->COR.Faixa[diagREGULAR];
	panCorV3->Color = geralTemp->COR.Faixa[diagBOM];
	panCorV4->Color = geralTemp->COR.Faixa[diagREGULAR];
	panCorV5->Color = geralTemp->COR.Faixa[diagRUIM];
	// cores para correntes
	panCorI1->Color = geralTemp->COR.Faixa[diagBOM];
	panCorI2->Color = geralTemp->COR.Faixa[diagREGULAR];
	panCorI3->Color = geralTemp->COR.Faixa[diagRUIM];
	// cores para correntes
	panCorS1->Color = geralTemp->COR.Faixa[diagBOM];
	panCorS2->Color = geralTemp->COR.Faixa[diagREGULAR];
	panCorS3->Color = geralTemp->COR.Faixa[diagRUIM];
}

// ---------------------------------------------------------------------------
TFormChild* __fastcall TFormCfgFlow::FormChildSelecionado(void)
{
	TFormChild *form;
	int index;

	index = cboxBaseSelecionada->ItemIndex;
	if (index >= 0)
	{
		form = (TFormChild*)cboxBaseSelecionada->Items->Objects[index];
	}

	return form;
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::FormShow(TObject *Sender)
{
	// apresenta dados na tela
	ExibeDados();
}

//// ---------------------------------------------------------------------------
//bool __fastcall TFormCfgFlow::LeDados(void)
//{
//	// variáveis locais
//	int soma;
//	TList *lisPADRAO;
//	VTPadraoRede *padrao;
//	int tipoSegmento = -1;
//
//	tipoSegmento = TipoRedeTabTensao();
//	try
//	{ // faixa A de Tensão
//		geral->FLOW.V[tipoSegmento][1] = StrToDouble(edtV1->Text);
//		geral->FLOW.V[tipoSegmento][2] = StrToDouble(edtV2->Text);
//		// faixa B de Tensão
//		geral->FLOW.V[tipoSegmento][0] = StrToDouble(edtV3->Text);
//		geral->FLOW.V[tipoSegmento][3] = StrToDouble(edtV4->Text);
//		// intervalos de Corrente
//		geral->FLOW.I[0] = StrToDouble(edtI1->Text) / 100;
//		geral->FLOW.I[1] = StrToDouble(edtI2->Text) / 100;
//		// intervalos de potência de trafo
//		geral->FLOW.S[0] = StrToDouble(edtS1->Text) / 100;
//		geral->FLOW.S[1] = StrToDouble(edtS2->Text) / 100;
//		// salva cores para tensões
//		// geral->COR.Faixa[diagRUIM]    = panCorV1->Color;
//		// geral->COR.Faixa[diagREGULAR] = panCorV2->Color;
//		// geral->COR.Faixa[diagBOM]     = panCorV3->Color;
//		// geral->COR.Faixa[diagREGULAR] = panCorV4->Color;
//		// geral->COR.Faixa[diagRUIM]    = panCorV5->Color;
//		// salva cores para cargto de chaves e trechos
//		// geral->COR.Faixa[diagBOM]     = panCorI1->Color;
//		// geral->COR.Faixa[diagREGULAR] = panCorI2->Color;
//		// geral->COR.Faixa[diagRUIM]    = panCorI3->Color;
//		// salva cores para cargto de trafos
//		// geral->COR.Faixa[diagBOM]     = panCorS1->Color;
//		// geral->COR.Faixa[diagREGULAR] = panCorS2->Color;
//		// geral->COR.Faixa[diagRUIM]    = panCorS3->Color;
//		geral->COR.Faixa[diagBOM] = PanelCorDiagBom->Color;
//		geral->COR.Faixa[diagREGULAR] = PanelCorDiagRegular->Color;
//		geral->COR.Faixa[diagRUIM] = PanelCorDiagRuim->Color;
//		// valida faixas de tensão
//		if ((geral->FLOW.V[tipoSegmento][0] < 0) ||
//			(geral->FLOW.V[tipoSegmento][0] > geral->FLOW.V[tipoSegmento][1]) ||
//			(geral->FLOW.V[tipoSegmento][1] > geral->FLOW.V[tipoSegmento][2]) ||
//			(geral->FLOW.V[tipoSegmento][2] > geral->FLOW.V[tipoSegmento][3]))
//		{
//			Erro("Faixas de tensão: valores inválidos");
//			return (false);
//		}
//		if ((geral->FLOW.I[0] < 0) || (geral->FLOW.I[0] > geral->FLOW.I[1]))
//		{
//			Erro("Faixas de corrente: valores inválidos");
//			return (false);
//		}
//		if ((geral->FLOW.S[0] < 0) || (geral->FLOW.S[0] > geral->FLOW.S[1]))
//		{
//			Erro("Faixas de potência: valores inválidos");
//			return (false);
//		}
//		// atualiza todos PadraoRede existentes
//		lisPADRAO = geral->LisPadraoRede();
//		for (int n = 0; n < lisPADRAO->Count; n++)
//		{
//			padrao = (VTPadraoRede*)lisPADRAO->Items[n];
//			try
//			{
//				padrao->V1 = geral->FLOW.V[padrao->PadraoRedeId][0];
//				padrao->V2 = geral->FLOW.V[padrao->PadraoRedeId][1];
//				padrao->V3 = geral->FLOW.V[padrao->PadraoRedeId][2];
//				padrao->V4 = geral->FLOW.V[padrao->PadraoRedeId][3];
//            }
//			catch(Exception &e)
//			{ //teste FKM
//				padrao->V1   = 0.90;
//				padrao->V2   = 0.93;
//				padrao->V3   = 1.05;
//				padrao->V4   = 1.05;
//			}
//			padrao->FPI1 = geral->FLOW.I[0];
//			padrao->FPI2 = geral->FLOW.I[1];
//			padrao->FPS1 = geral->FLOW.S[0]; // DVK 2016.07.07
//			padrao->FPS2 = geral->FLOW.S[1]; // DVK 2016.07.07
//		}
//	}
//	catch (EConvertError &exception)
//	{
//		Erro("Valores numéricos definidos com caracteres inválidos");
//		return (false);
//	}
//	return (true);
//}// ---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::LeDados(void)
{
	// variáveis locais
	int soma;
	TList *lisPADRAO;
	VTPadraoRede *padrao;
	int tipoSegmento = -1;

	tipoSegmento = TipoRedeTabTensao();
	try
	{
		AtualizaValoresTemp(tcFaixaV->TabIndex);
		// intervalos de Corrente
		geralTemp->FLOW.I[0] = StrToDouble(edtI1->Text) / 100;
		geralTemp->FLOW.I[1] = StrToDouble(edtI2->Text) / 100;
		// intervalos de potência de trafo
		geralTemp->FLOW.S[0] = StrToDouble(edtS1->Text) / 100;
		geralTemp->FLOW.S[1] = StrToDouble(edtS2->Text) / 100;
		// salva cores para tensões
		geralTemp->COR.Faixa[diagBOM] = PanelCorDiagBom->Color;
		geralTemp->COR.Faixa[diagREGULAR] = PanelCorDiagRegular->Color;
		geralTemp->COR.Faixa[diagRUIM] = PanelCorDiagRuim->Color;
		// valida faixas de tensão
		if(ValidaValoresTemp() == false)
			{return false;}
		if ((geralTemp->FLOW.I[0] < 0) || (geralTemp->FLOW.I[0] > geralTemp->FLOW.I[1]))
		{
			Erro("Faixas de corrente: valores inválidos");
			return (false);
		}
		if ((geralTemp->FLOW.S[0] < 0) || (geralTemp->FLOW.S[0] > geralTemp->FLOW.S[1]))
		{
			Erro("Faixas de potência: valores inválidos");
			return (false);
		}
		// atualiza todos PadraoRede existentes
		lisPADRAO = geralTemp->LisPadraoRede();
		for (int n = 0; n < lisPADRAO->Count; n++)
		{
			padrao = (VTPadraoRede*)lisPADRAO->Items[n];
			padrao->V1   = 0.90;
			padrao->V2   = 0.93;
			padrao->V3   = 1.05;
			padrao->V4   = 1.05;
			padrao->FPI1 = geralTemp->FLOW.I[0];
			padrao->FPI2 = geralTemp->FLOW.I[1];
			padrao->FPS1 = geralTemp->FLOW.S[0]; // DVK 2016.07.07
			padrao->FPS2 = geralTemp->FLOW.S[1]; // DVK 2016.07.07
		}
	}
	catch (EConvertError &exception)
	{
		Erro("Valores numéricos definidos com caracteres inválidos");
		return (false);
	}
	return (true);
}
// -----------------------------------------------------------------------------
AnsiString __fastcall TFormCfgFlow::NomeBase(TFormChild *pFormChild)
{
	VTPath *path;
	VTApl *cApl;
	AnsiString filename, filepath;

	cApl = pFormChild->Apl;
	if (cApl == NULL)
		{return "";}
	path =  (VTPath*)cApl->GetObject(__classid(VTPath));
	filepath = path->ArqRede;
	filename = "(Rede " + IntToStr(pFormChild->Tag) +") " +  ExtractFileName(filepath);

	return filename;
}
// -----------------------------------------------------------------------------
void __fastcall TFormCfgFlow::PanelCorDblClick(TObject *Sender)
{
	// variáveis locais
	TPanel *Panel = (TPanel*)Sender;

	// seleciona nova cor
	ColorDialog->Color = Panel->Color;
	if (ColorDialog->Execute())
	{ // salva nova cor do circuito
		Panel->Color = ColorDialog->Color;
		// atualiza objeto Geral
		if (Panel == PanelCorDiagBom)
			geralTemp->COR.Faixa[diagBOM] = Panel->Color;
		else if (Panel == PanelCorDiagRegular)
			geralTemp->COR.Faixa[diagREGULAR] = Panel->Color;
		else if (Panel == PanelCorDiagRuim)
			geralTemp->COR.Faixa[diagRUIM] = Panel->Color;
		// atualiza cores dos Panel
		ExibeCores();
	}
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormCfgFlow::ReiniciaValoresDefault(void)
 {
 //intervalos de Tensão
 geral->FLOW.V[0] = 0.90;
 geral->FLOW.V[1] = 0.93;
 geral->FLOW.V[2] = 1.05;
 geral->FLOW.V[3] = 1.05;
 //intervalos de cargto de Cabo
 geral->FLOW.I[0] = 0.7;
 geral->FLOW.I[1] = 1.0;
 //intervalos de cargto de Trafo
 geral->FLOW.S[0] = 0.8;
 geral->FLOW.S[1] = 1.0;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::SalvaValoresTemp(VTGeral *pGeral)
{
	VTPadraoRede *padraoRede;

	//carrega valores redeSEC
	padraoRede = pGeral->ExistePadraoRede(redeSEC);
	if(padraoRede != NULL)
	{
		padraoRede->V1= valoresTemp[tabCFGV_REDESEC][0];
		padraoRede->V2= valoresTemp[tabCFGV_REDESEC][1];
		padraoRede->V3= valoresTemp[tabCFGV_REDESEC][2];
		padraoRede->V4= valoresTemp[tabCFGV_REDESEC][3];
	}
	//carrega valores redePRI
	padraoRede = pGeral->ExistePadraoRede(redePRI);
	if(padraoRede != NULL)
	{
		padraoRede->V1 = valoresTemp[tabCFGV_REDEPRI][0];
		padraoRede->V2 = valoresTemp[tabCFGV_REDEPRI][1];
		padraoRede->V3 = valoresTemp[tabCFGV_REDEPRI][2];
		padraoRede->V4 = valoresTemp[tabCFGV_REDEPRI][3];
	}
	//carrega valores redeSUB
	padraoRede = pGeral->ExistePadraoRede(redeSUB);
	if(padraoRede != NULL)
	{
		padraoRede->V1 = valoresTemp[tabCFGV_REDESUB][0];
		padraoRede->V2 = valoresTemp[tabCFGV_REDESUB][1];
		padraoRede->V3 = valoresTemp[tabCFGV_REDESUB][2];
		padraoRede->V4 = valoresTemp[tabCFGV_REDESUB][3];
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::SelecionaBaseFormChild(TFormChild *form)
{
	int index =-1;
	TFormChild *child;

	//se nao tiver formChild e o cbox tiver sido inicializado
	if (form == NULL)
	{
		if (cboxBaseSelecionada->Items->Count > 0)
		{   //seleciona a 1a opcao (bases futuras)
			index = 0;
		}
	}
	else
	{   //verifica o indice do form no combo
		for (int n=0; n < cboxBaseSelecionada->Items->Count; n++)
		{
			child = (TFormChild*)cboxBaseSelecionada->Items->Objects[n];
			if (form == child)
			{
				index = n;
				break;
			}
		}
	}
	if (index >= 0)
	{
        cboxBaseSelecionada->ItemIndex = index;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::tcFaixaVChange(TObject *Sender)
{
	AtualizaValoresTemp(tabAnterior);
	ExibeDadosTensao();
	tabAnterior = tcFaixaV->TabIndex;
}
//---------------------------------------------------------------------------
int  __fastcall TFormCfgFlow::TipoRedeTabTensao(void)
{
	int tipoSegmento = -1;

	switch(tcFaixaV->TabIndex)
	{
        case tabCFGV_REDESEC:
			tipoSegmento = redeSEC;
			break;
		case tabCFGV_REDEPRI:
			tipoSegmento = redePRI;
			break;
		case tabCFGV_REDESUB:
			tipoSegmento = redeSUB;
			break;
		default:
			break;
	}

	return tipoSegmento;
}
//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::ValidaValoresTemp(void)
{
	//percorre cada conjunto de valores

	// valida faixas de tensão redeSEC
	if ((valoresTemp[tabCFGV_REDESEC][0] < 0) ||
		(valoresTemp[tabCFGV_REDESEC][0] > valoresTemp[tabCFGV_REDESEC][1]) ||
		(valoresTemp[tabCFGV_REDESEC][1] > valoresTemp[tabCFGV_REDESEC][2]) ||
		(valoresTemp[tabCFGV_REDESEC][2] > valoresTemp[tabCFGV_REDESEC][3]))
	{
		Erro("Faixas de tensão BT: valores inválidos");
		return (false);
	}
	// valida faixas de tensão redePRI
	if ((valoresTemp[tabCFGV_REDEPRI][0] < 0) ||
		(valoresTemp[tabCFGV_REDEPRI][0] > valoresTemp[tabCFGV_REDEPRI][1]) ||
		(valoresTemp[tabCFGV_REDEPRI][1] > valoresTemp[tabCFGV_REDEPRI][2]) ||
		(valoresTemp[tabCFGV_REDEPRI][2] > valoresTemp[tabCFGV_REDEPRI][3]))
	{
		Erro("Faixas de tensão MT: valores inválidos");
		return (false);
	}
	// valida faixas de tensão redeSUB
	if ((valoresTemp[tabCFGV_REDESUB][0] < 0) ||
		(valoresTemp[tabCFGV_REDESUB][0] > valoresTemp[tabCFGV_REDESUB][1]) ||
		(valoresTemp[tabCFGV_REDESUB][1] > valoresTemp[tabCFGV_REDESUB][2]) ||
		(valoresTemp[tabCFGV_REDESUB][2] > valoresTemp[tabCFGV_REDESUB][3]))
	{
		Erro("Faixas de tensão AT: valores inválidos");
		return (false);
	}

    return true;
}
//---------------------------------------------------------------------------
// eof








