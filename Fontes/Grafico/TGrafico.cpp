// -----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDestaque.h"
#include "TGrafico.h"
#include "TFormGraf.h"
#include "TPadrao.h"
#include "VTAnimacao.h"
#include "VTUnif.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\VTEdt.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

// -----------------------------------------------------------------------------
VTGrafico* __fastcall NewObjGrafico(VTApl *apl)
{
	return (new TGrafico(apl));
}

// -----------------------------------------------------------------------------
__fastcall TGrafico::TGrafico(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// desabilita uso de mapa
	mapa_enabled = false;
	// cria objeto TPadrao
	padrao = NewObjPadrao();
	// cria forms formCfgPad e TformGraf
	formGraf = new TFormGraf(apl->Form, this, apl);
	// define este objeto VTGrafico para TformGraf
	// formGraf->DefObjGraf(this);
	// cria objeto p/ controlar animação
	animacao = NewObjAnimacao();
	// cria área p/ leitura da área de zoom
	area_tmp = DLL_NewObjArea();
	// cria listas
	lisUNIF = new TList();
	lisEDT = new TList();
	// cria lista p/ objetos c/ moldura
	moldura.lisEQP = new TList();
	moldura.lisSOL = new TList();
	// cria lista p/ objetos em destaque
	// destaque.lisEQP = new TList();
	destaque = new TDestaque();
	lisDESTAQUE = new TList();
	// cria objetos VTUnif que irão desenhar o diagrama unifilar da rede
	IniciaLisUnif();
}

// -----------------------------------------------------------------------------
__fastcall TGrafico::~TGrafico(void)
{
	Close();
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::AreaRedeAltera(void)
{
	// variáveis locais
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia conjunto de Celulas
	celulas->DefineAreaTotal();
	// redefine conjunto de Celulas ativas pois FormFgraf NÃO irá chamar AreaZoomAlterada
	celulas->DefineAreaAtiva(AreaVisivel);
	// redefine área da rede p/ FormGraf
	formGraf->AreaRedeAltera(redegraf->Area(redes));
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::AreaRedeDefine(void)
{
	// variáveis locais
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia conjunto de Celulas
	celulas->DefineAreaTotal();
	// NÃO redefine conjunto de Celulas ativas pois FormFgraf irá chamar AreaZoomAlterada
	// celulas->DefineAreaAtiva(AreaVisivel);
	// define área da rede p/ FormGraf
	formGraf->AreaRedeDefine(redegraf->Area(redes));
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::AreaZoomAlterada(void)
{
	// variáveis locais
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

	// redefine conjunto de Celulas ativas
	celulas->DefineAreaAtiva(AreaVisivel);
	// verifica se foi definida ActionOnZoomChange p/ tratamento externo de alteração de zoom
	if (ActionOnZoomChange != NULL)
		ActionOnZoomChange->Execute();
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Close(void)
{
	// variáveis locais
	TFormGraf *form_graf_tmp = formGraf;

	// desabilita gráfico (cancelando Unif selecionado)
	Disable();
	// cancela ponteiro antes  de destruir FormGraf
	formGraf = NULL;
	// destrói TformGraf criado pelo construtor
	// if (form_graf_tmp) {form_graf_tmp->Parent = NULL; delete form_graf_tmp;}
	if (form_graf_tmp)
	{
		delete form_graf_tmp;
	}
	// destrói objetos
	if (animacao)
	{
		delete animacao;
		animacao = NULL;
	}
	if (padrao)
	{
		delete padrao;
		padrao = NULL;
	}
	if (area_tmp)
	{
		delete area_tmp;
		area_tmp = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEDT)
	{
		delete lisEDT;
		lisEDT = NULL;
	}
	if (moldura.lisEQP)
	{
		delete moldura.lisEQP;
		moldura.lisEQP = NULL;
	}
	if (moldura.lisSOL)
	{
		delete moldura.lisSOL;
		moldura.lisSOL = NULL;
	}
	// if (destaque.lisEQP)
	// {
	// delete destaque.lisEQP;
	// destaque.lisEQP = NULL;
	// }
	if (destaque)
	{
		delete destaque;
		destaque = NULL;
	}
	// destrói lista e seus objetos
	if (lisUNIF)
	{
		LimpaTList(lisUNIF);
		delete lisUNIF;
		lisUNIF = NULL;
	}
	if (lisDESTAQUE)
	{
		LimpaTList(lisDESTAQUE);
		delete lisDESTAQUE;
		lisDESTAQUE = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Cursor(TCursor cursor)
{
	// atualiza formato do cursor do mouse
	if (formGraf)
		formGraf->ImageCursorMouse(cursor);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::DefActionOnZoomChange(TAction *ActionOnZoomChange)
{
	// salva Action externa
	this->ActionOnZoomChange = ActionOnZoomChange;
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::DefMouseOp(int op)
{
	formGraf->DefMouseOp(op);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::DefineParent(TWinControl *parent)
{
	// redefine Parent dos Forms
	formGraf->DefineParent(parent);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::DefineParentToolBar(TWinControl *parent)
{
	// proteção
	if (parent == NULL)
	{
		formGraf->ToolBar->Visible = false;
		return;
	}
	// redefine Parent do ToolBar do FormGraf
	formGraf->ToolBar->Parent = parent;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
void __fastcall TGrafico::DestacaBarras(TList *lisEQP, TColor color, int dim_linha)
{
	// variaveis locais
	TDestaque *dest;

	// proteção
	if (unif == NULL)
		return;
	// proteção
	if ((lisEQP == NULL) || (lisEQP->Count == 0))
	{
		return;
	}
	if (dim_linha < 1)
		dim_linha = 1;
	if (dim_linha > 5)
		dim_linha = 5;
	try
	{ // cria objeto destaque
		dest = new TDestaque();
		CopiaTList(lisEQP, dest->lisEQP);
		dest->color = color;
		dest->dim = dim_linha;
		lisDESTAQUE->Add(dest);
		// reapresenta o gráfico
//		Redraw();
	}
	catch (...)
	{
		if (dest != NULL)
		{
			lisDESTAQUE->Remove(dest);
			delete dest;
		}
	}
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TGrafico::DestacaEqpto(TList *lisEQP, TColor color, int dim_linha)
 {
 // proteção
 if (unif == NULL)
 return;
 // proteção
 if ((lisEQP == NULL) || (lisEQP->Count == 0))
 { // verifica se havia Eqptos em destaque
 if (destaque.lisEQP->Count == 0)
 return;
 }
 // reinicia lista de Eqptos em destaque
 destaque.lisEQP->Clear();
 // copia Eqptos em destaque
 if (lisEQP)
 { // proteção
 if (dim_linha < 1)
 dim_linha = 1;
 if (dim_linha > 5)
 dim_linha = 5;
 // salva dados p/ destaque de eqptos
 CopiaTList(lisEQP, destaque.lisEQP);
 destaque.color = color;
 destaque.dim = dim_linha;
 }
 // reapresenta o gráfico
 Redraw();
 // unif->DestacaEqpto(destaque.lisEQP, color, fator);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TGrafico::DestacaEqpto(TList *lisEQP, TColor color, int dim_linha)
{
	// proteção
	if (unif == NULL)
		return;
	// proteção
	if ((lisEQP == NULL) || (lisEQP->Count == 0))
	{ // verifica se havia Eqptos em destaque
		if (destaque->lisEQP->Count == 0)
			return;
	}
	// reinicia lista de Eqptos em destaque
	destaque->lisEQP->Clear();
	// copia Eqptos em destaque
	if (lisEQP)
	{ // proteção
		if (dim_linha < 1)
			dim_linha = 1;
		if (dim_linha > 5)
			dim_linha = 5;
		// salva dados p/ destaque de eqptos
		CopiaTList(lisEQP, destaque->lisEQP);
		destaque->color = color;
		destaque->dim = dim_linha;
	}
	// reapresenta o gráfico
	Redraw();
	// unif->DestacaEqpto(destaque.lisEQP, color, fator);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Disable(void)
{
	unif = NULL;
}

// -----------------------------------------------------------------------------
double __fastcall TGrafico::Escala(void)
{
	return (formGraf->Escala());
}

// -----------------------------------------------------------------------------
VTUnif* __fastcall TGrafico::ExisteUnif(int graf_num)
{
	// variáveis locais
	VTUnif *unif;

	// proteção
	if (graf_num == grafNONE)
		return (NULL);
	// verifica se existe o Unif indicado
	for (int n = 0; n < lisUNIF->Count; n++)
	{
		unif = (VTUnif*)lisUNIF->Items[n];
		if (unif->Tipo() == graf_num)
			return (unif);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
bool __fastcall TGrafico::Exporta(TCanvas *canvas, TRect &rect, TColor cor_fundo, int opcao)
{
	// desenha gráfico no Canvas indicado
	return (formGraf->Exporta(canvas, rect));
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Hide(void)
{
	// esconde o form
	formGraf->Hide();
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TGrafico::IniciaLisUnif(void)
 {
 //variáveis locais
 VTUnif *unif;

 if ((unif = NewObjUnifTopo(apl,      grafTOPO,      "Topologia"))      != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifFlow(apl,      grafFLOW,      "Fluxo ativo"))    != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifCurto(apl,     grafCURTO,     "Curto circuito")) != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifConfiab(apl,   grafCONFIAB,   "Confiabilidade")) != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifCabo(apl,      grafARRANJO,   "Arranjos"))       != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifFase(apl,      grafFASE,      "Fases"))          != NULL) lisUNIF->Add(unif);
 if ((unif = NewObjUnifRegulador(apl, grafREGULADOR, "Regulador"))      != NULL) lisUNIF->Add(unif);
 //inicia objetos Unif
 for (int n = 0; n < lisUNIF->Count; n++)
 {
 unif = (VTUnif*)lisUNIF->Items[n];
 unif->DefObjFormGraf(formGraf);
 unif->DefObjAnimacao(animacao);
 unif->DefObjPadrao(padrao);
 }
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TGrafico::IniciaLisUnif(void)
{
	// variáveis locais
	VTUnif *unif;

	InsereObjUnif(apl, grafTOPO);
	InsereObjUnif(apl, grafFLOW);
	InsereObjUnif(apl, grafCURTO);
	InsereObjUnif(apl, grafCONFIAB);
	InsereObjUnif(apl, grafARRANJO);
	InsereObjUnif(apl, grafFASE);
	InsereObjUnif(apl, grafREGULADOR);
	InsereObjUnif(apl, grafTOPO_BP);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::InsereObjEditor(VTEdt *edt)
{
	// variáveis locais
	VTEdt *ult_edt;

	// proteção
	if (edt == NULL)
		return;
	// verifica se existia outros objeto VTEdt em lisEDT
	if (lisEDT->Count > 0)
	{ // retira o foco do último objeto VTEdt em lisEDT
		ult_edt = (VTEdt*)lisEDT->Last();
		ult_edt->ResetGrafFocus();
	}
	// insere novo objeto VTEdt em lisEDT
	if (lisEDT->IndexOf(edt) < 0)
		lisEDT->Add(edt);
}

// -----------------------------------------------------------------------------
VTUnif* __fastcall TGrafico::InsereObjUnif(VTApl *apl, int graf_num)
{
	// variáveis locais
	VTUnif *unif;

	switch (graf_num)
	{
	case grafTOPO:
		unif = NewObjUnifTopo(apl, grafTOPO, "Topologia");
		break;
	case grafFLOW:
		unif = NewObjUnifFlow(apl, grafFLOW, "Fluxo ativo");
		break;
	case grafCURTO:
		unif = NewObjUnifCurto(apl, grafCURTO, "Curto circuito");
		break;
	case grafCONFIAB:
		unif = NewObjUnifConfiab(apl, grafCONFIAB, "Confiabilidade");
		break;
	case grafARRANJO:
		unif = NewObjUnifCabo(apl, grafARRANJO, "Arranjos");
		break;
	case grafFASE:
		unif = NewObjUnifFase(apl, grafFASE, "Fase");
		break;
	case grafREGULADOR:
		unif = NewObjUnifRegulador(apl, grafREGULADOR, "Regulador");
		break;
	case grafDIAG:
		unif = NewObjUnifDiag(apl, grafDIAG, "Diagnóstico");
		break;
	case grafTOPO_BP:
		unif = NewObjUnifTopobp(apl, grafTOPO_BP, "Topologia");
		break;
	default:
		unif = NULL;
	}
	// inicia objeto Unif
	if (unif != NULL)
	{
		unif->DefObjFormGraf(formGraf);
		unif->DefObjAnimacao(animacao);
		unif->DefObjPadrao(padrao);
		// insere Unif em lisUNIF
		lisUNIF->Add(unif);
	}
	return (unif);
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TGrafico::InsereObjUnif(VTUnif *unif)
 {
 //proteção
 if (unif == NULL) return;
 //insere objeto Unif em lisUNIF
 lisUNIF->Add(unif);
 //inicia objetos Unif
 unif->DefObjFormGraf(formGraf);
 unif->DefObjAnimacao(animacao);
 unif->DefObjPadrao(padrao);
 }
 */
// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
void __fastcall TGrafico::LimpaDestaque(void)
{
	//limpa lista de destaques
	LimpaTList(lisDESTAQUE);
    // reapresenta o gráfico
	Redraw();
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::ModoGraficoAlterado(void)
{
	// variáveis locais
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia conjunto de Celulas
	celulas->DefineAreaTotal();
	// habilita/desabilita exibição da cartografia
	formGraf->MapaEnabled(mapa_enabled & (redegraf->ModoGrafico == modoGEOREFERENCIADO));
	// redefine área da rede p/ FormGraf (o que força a reapresentação da rede)
	formGraf->AreaRedeDefine(redegraf->Area(redes));
}

// ---------------------------------------------------------------------------
void __fastcall TGrafico::Moldura(VTEqpto *eqpto)
{
	// proteção
	if (unif == NULL)
		return;
	// se já existe eqpto c/ moldura, apaga
	unif->Moldura(moldura.lisEQP);
	// reinicia lista de Eqptos c/ moldura
	moldura.lisEQP->Clear();
	if (eqpto == NULL)
		return;
	// insere Eqpto c/ moldura
	moldura.lisEQP->Add(eqpto);
	// exibe moldura do Eqpto
	unif->Moldura(moldura.lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TGrafico::Moldura(TList *lisEQP)
{
	// proteção
	if (unif == NULL)
		return;
	// se já existe Eqpto c/ moldura, apaga moldura
	unif->Moldura(moldura.lisEQP);
	// reinicia lista de Eqptos c/ moldura
	moldura.lisEQP->Clear();
	CopiaTList(lisEQP, moldura.lisEQP);
	// exibe moldura dos Eqptos
	unif->Moldura(moldura.lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TGrafico::MolduraSolida(TList *lisEQP, TColor color)
{
	// proteção
	if (unif == NULL)
		return;
	// salva lista de Eqptos c/ moldura sólida
	moldura.color = color;
	moldura.lisSOL->Clear();
	if ((lisEQP != NULL) && (lisEQP->Count > 0))
		CopiaTList(lisEQP, moldura.lisSOL);
	// reapresenta o gráfico
	Redraw();
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Mouse(int Action, TMouseButton Button, TShiftState Shift, int x_utm,
	int y_utm, int x, int y)
{
	// variáveis locais
	VTEdt *edt;

	// passa o evento p/ o último VTEdt da lista lisEDT
	if (lisEDT->Count > 0)
	{
		edt = (VTEdt*)lisEDT->Last();
		edt->EvtMouse(Action, Button, Shift, x_utm, y_utm, x, y);
	}
}
/*
 //---------------------------------------------------------------------------
 TPadrao* __fastcall TGrafico::Padrao(void)
 {
 return(padrao);
 }
 */

// -----------------------------------------------------------------------------
VTApl* __fastcall TGrafico::PM_GetApl(void)
{
	return (apl);
}

// -----------------------------------------------------------------------------
VTArea* __fastcall TGrafico::PM_GetAreaTotal(void)
{
	// obtém área total de TFormGraf
	formGraf->AreaTotal(area_tmp);
	return (area_tmp);
}

// -----------------------------------------------------------------------------
VTArea* __fastcall TGrafico::PM_GetAreaVisivel(void)
{
	// obtém área de zoom de TFormGraf
	formGraf->AreaVisivel(area_tmp);
	return (area_tmp);
}

// -----------------------------------------------------------------------------
VTArea* __fastcall TGrafico::PM_GetAreaZoom(void)
{
	// obtém área de zoom de TFormGraf
	formGraf->AreaZoom(area_tmp);
	return (area_tmp);
}
// -----------------------------------------------------------------------------
TCanvas* __fastcall TGrafico::PM_GetCanvas(void)
{
    return(formGraf->Canvas);
}
// -----------------------------------------------------------------------------
TPadrao* __fastcall TGrafico::PM_GetPadrao(void)
{
	return (padrao);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::PM_SetAnimacaoEnabled(bool enabled)
{
	// atualiza objeto Animacao
	animacao->Enabled = enabled;
	// atualiza objeto Unif
	if (unif)
		unif->AnimacaoEnabled = enabled;
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::PM_SetMapaEnabled(bool enabled)
{
	// variáveis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// salva controle de uso do mapa
	mapa_enabled = enabled;
	// habilita/desabilita exibição da cartografia
	formGraf->MapaEnabled(mapa_enabled & (redegraf->ModoGrafico == modoGEOREFERENCIADO));
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TGrafico::PM_SetMapaTematicoEnabled(bool enabled)
 {
 if (unif)
 {
 unif->MapaTematicoEnabled = enabled;
 //atualiza o gráfico
 Redraw();
 }
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TGrafico::PM_SetMapaTematicoEnabled(bool enabled)
{
	// variáveis locais
	VTUnif *obj_unif;

	// configura todos objetos Unif
	for (int n = 0; n < lisUNIF->Count; n++)
	{
		obj_unif = (VTUnif*)lisUNIF->Items[n];
		obj_unif->MapaTematicoEnabled = enabled;
	}
	// verifica se há um objeto Unif ativo
	if (unif)
	{ // atualiza o gráfico
		Redraw();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::PM_SetAreaZoom(VTArea *area)
{
	// define área de zoom de TFormGraf
	formGraf->ZoomDefineArea(area->x1, area->y1, area->x2, area->y2);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Redraw(void)
{
	// variáveis locais
	VTEdt *edt = NULL;
	TDestaque *dest;

	///proteção
	if (unif == NULL)
		return;
	// define VTEdt ativo
	if (lisEDT->Count > 0)
		edt = (VTEdt*)lisEDT->Last();
	// proteção necessária na iniciação
	if ((!formGraf) || (!unif))
		return;
	// se o form não está visível, não faz nada
	if (!formGraf->Visible)
		return;
	// desenha o diag.unifilar
	unif->Redraw(edt);
	// apresenta moldura
	unif->Moldura(moldura.lisEQP);
	// apresenta moldura sólida
	unif->MolduraSolida(moldura.lisSOL, moldura.color);
	// exibe Eqptos em destaque
	// unif->DestacaEqpto(destaque.lisEQP, destaque.color, destaque.dim);
	unif->DestacaEqpto(destaque->lisEQP, destaque->color, destaque->dim);
	for(int n = 0; n <lisDESTAQUE->Count; n++)
	{
		dest = (TDestaque*)lisDESTAQUE->Items[n];
		unif->DestacaEqpto(dest->lisEQP, dest->color, dest->dim);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Redraw(int op, VTEdt *editor)
{
	// proteção necessária na iniciação
	if ((!formGraf) || (!unif))
		return;
	// se o form não está visível, não faz nada
	if (!formGraf->Visible)
		return;
	// desenha o diag.unifilar
	unif->Redraw(op, editor);
	// JCG 2011.05.19
	// apresenta moldura
	// unif->Moldura(moldura.lisEQP);
	// exibe Eqptos em destaque
	// unif->DestacaEqpto(moldura.lisEQP, destaque.color, destaque.fator);
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Refresh(void)
{
	// esconde o form
	if (formGraf->Visible)
		formGraf->imag->Refresh();
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::RetiraObjEditor(VTEdt *edt)
{
	// variáveis locais
	int index;

	// verifica se o objeto VTEdt existe em lisEDT
	if ((index = lisEDT->IndexOf(edt)) < 0)
		return;
	// retira objeto da lista
	lisEDT->Delete(index);
	// verifica se o VTEdt retirado estava c/ o foco
	if (index == lisEDT->Count)
	{ // verifica se há outro objeto VTEdt em lisEDT
		if (lisEDT->Count > 0)
		{ // passa o foco p/ o último objeto VTEdt em lisEDT
			edt = (VTEdt*)lisEDT->Last();
			edt->SetGrafFocus();
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::RetiraObjUnif(VTUnif *obj_unif)
{
	// proteção
	if (obj_unif == NULL)
		return;
	// verifica se é o Unif corrente
	if (obj_unif == unif)
	{ // cancela o gráfico
		Disable();
		Hide();
	}
	// retira obj_unif da lista e destrói objeto
	lisUNIF->Remove(obj_unif);
	delete obj_unif;
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::SelArea(int x1, int y1, int x2, int y2)
{
	// variáveis locais
	VTEdt *edt;

	// passa o evento p/ o último VTEdt da lista lisEDT
	if (lisEDT->Count > 0)
	{
		edt = (VTEdt*)lisEDT->Last();
		edt->EvtSelArea(x1, y1, x2, y2);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::SelecionaZoom(VTArea *area_utm, VTArea *area_esq)
{
	// variáveis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica o modo gráfico em uso
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
	{
		formGraf->ZoomDefineArea(area_utm->x1, area_utm->y1, area_utm->x2, area_utm->y2);
	}
	else
	{
		formGraf->ZoomDefineArea(area_esq->x1, area_esq->y1, area_esq->x2, area_esq->y2);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::SelecionaZoom(VTEqpto *eqpto, bool ampliar)
{
	// variáveis locais
	int x1, y1, x2, y2, dx, dy;
	VTArea *area;
	VTCoordenada *coordenada;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se é igual ao último Eqpto usado para Zoom
	if ((eqpto != NULL) && (eqpto == zoom.eqpto))
	{ // redefine área em torno do Eqpto para ampliar ou diminuir imagem
		if (ampliar)
			zoom.fator = zoom.fator / 2;
		else
			zoom.fator = zoom.fator * 2;
	}
	else
	{
		zoom.eqpto = eqpto;
		zoom.fator = 0.10;
	}
	// proteção
	if (eqpto == NULL)
	{ // cancela zoom
		area = redegraf->Area(redes);
		formGraf->ZoomDefineArea(area->x1, area->y1, area->x2, area->y2);
		return;
	}
	// verifica se Eqpto é uma Rede
	if (eqpto->Tipo() == eqptoREDE)
	{ // determina área da Rede
		area = redegraf->Area((VTRede*)eqpto);
		// define área de zoom igual à área da Rede
		formGraf->ZoomDefineArea(area->x1, area->y1, area->x2, area->y2);
		return;
	}
	// determina área de Redes
	area = redegraf->Area(redes);
	// obtém coordenadas do Eqpto
	coordenada = redegraf->CoordEqpto[eqpto];
	if (coordenada->Line)
	{ // define área de zoom igual a 20% da área total em torno do Eqpto
		dx = area->Width() * zoom.fator;
		dy = area->Height() * zoom.fator;
		// if ((x1 = coordenada->Line->x1 - dx) < area->x1) x1 = area->x1;
		// if ((y1 = coordenada->Line->y1 - dy) < area->x1) y1 = area->y1;
		// if ((x2 = coordenada->Line->x2 + dx) < area->x1) x2 = area->x2;
		// if ((y2 = coordenada->Line->y2 + dy) < area->x1) y2 = area->y2;
		// DVK 2016.10.11 verifica coordenadas x e y
		if ((x1 = coordenada->Line->x1 - dx) < area->x1)
			x1 = area->x1;
		if ((y1 = coordenada->Line->y1 - dy) < area->y1)
			y1 = area->y1;
		if ((x2 = coordenada->Line->x2 + dx) < area->x1)
			x2 = area->x2;
		if ((y2 = coordenada->Line->y2 + dy) < area->y1)
			y2 = area->y2;

		formGraf->ZoomDefineArea(x1, y1, x2, y2);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::SizeOfString(AnsiString txt, int &width, int &height)
{
	width = formGraf->Canvas->TextWidth(txt);
	height = formGraf->Canvas->TextHeight(txt);
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TGrafico::Show(int graf_num, int ano_carga, int npat)
 {
 //proteção
 if ((ano_carga < 0)||(ano_carga >= MAX_ANO)) ano_carga = 0;
 //atualiza ano da carga e patamar em TPadrao
 padrao->flow.ano_carga = ano_carga;
 padrao->flow.num_pat   = npat;
 //verifica se deve deixar de exibir o gráfico
 if ((graf_num == grafNONE)||(graf_num < 0)||(graf_num >= lisUNIF->Count))
 {//limpa gráfico atual
 if (unif) unif->LimpaTela();
 unif = NULL;
 }
 else
 {//redefine o unifilar a ser exibido
 unif = (VTUnif*)lisUNIF->Items[graf_num];
 //verifica se FormGraf já está visível
 if (! formGraf->Visible)
 {//exibe FormGraf
 formGraf->Show();
 }
 //coloca FormGraf como último componente p/ não encobrir outras janelas
 formGraf->SendToBack();
 //atualiza gráfico
 Redraw();
 }
 }
 */
// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422280
// https://sinapsisenergia.teamwork.com/#tasks/15766971
void __fastcall TGrafico::Show(int graf_num, int ano_carga, int npat)
{
	// variáveis locais
	VTUnif *novo_unif;
	int num_pat_anterior, ano_carga_anterior;

	// proteção
	// if ((ano_carga < 0)||(ano_carga >= MAX_ANO)) ano_carga = 0;
	if (ano_carga < 0)
		ano_carga = 0;
	// atualiza ano da carga e patamar em TPadrao
	num_pat_anterior = padrao->flow.num_pat;
	ano_carga_anterior = padrao->flow.ano_carga;
	padrao->flow.ano_carga = ano_carga;
	padrao->flow.num_pat = npat;
	// determina novo Unif a ser utilizado
	if ((novo_unif = ExisteUnif(graf_num)) == NULL)
	{ // limpa gráfico atual
		if (unif)
			unif->LimpaTela();
		// cancela Unif
		unif = NULL;
	}
	else if ((novo_unif != unif) || ((num_pat_anterior != npat) ||
		(ano_carga_anterior != ano_carga)))
	{ // redefine Unif
		unif = novo_unif;
		// JCG 2019.05.14
		// verifica se FormGraf já está visível
		if (!formGraf->Visible)
		{ // exibe FormGraf
			formGraf->Show();
			// coloca FormGraf como último componente p/ não encobrir outras janelas
			formGraf->SendToBack();
		}
		else
		{ // atualiza gráfico
			Redraw();
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TGrafico::Show(AnsiString unif_nome, int ano_carga, int npat)
{
	// variáveis locais
	VTUnif *unif;

	// determina objeto Unif pelo seu nome
	for (int n = 0; n < lisUNIF->Count; n++)
	{
		unif = (VTUnif*)lisUNIF->Items[n];
		if (unif->Nome.AnsiCompareIC(unif_nome) == 0)
		{
			Show(n, ano_carga, npat);
			return;
		}
	}
	// não encontrou o objeto Unif: cancela exibição do gráfico
	Show(grafNONE, ano_carga, npat);
}

// ---------------------------------------------------------------------------
// eof
