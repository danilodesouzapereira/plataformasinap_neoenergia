// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifEditor.h"
#include "TPadrao.h"
#include "TFormGraf.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "VTGrafico.h"
#include "..\Editor\VTEdt.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTFigura.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Geral\VTGeral.h"
#include "..\Obra\VTObra.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Redegraf\VTRedegraf.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

/*
 //-----------------------------------------------------------------------------
 // função global
 //-----------------------------------------------------------------------------
 VTUnif* NewObjUnifEditor(VTApl *apl, AnsiString nome)
 {
 return(new TUnifEditor(apl, nome));
 }
 */
// ---------------------------------------------------------------------------
__fastcall TUnifEditor::TUnifEditor(VTApl *apl, int tipo_graf, AnsiString nome)
	: TUnif(apl, tipo_graf, nome)
{
	// define cores adicionais
	cor.eqpto_novo = clGray;
	cor.eqpto_sel = clRed;
	cor.marca = clGray;
}

// ---------------------------------------------------------------------------
__fastcall TUnifEditor::~TUnifEditor(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::Redraw(VTEdt *editor)
{
	// salva ponteiro para objeto
	this->editor = editor;
	// executa método da classe base
	TUnif::RedrawAll();
	// reapresenta eqptos em edição
	if (editor)
	{
		RedrawAllEqptoNovo();
		RedrawAllEqptoSelCorUnica();
		RedrawAllMarca();
	}
	// atualiza gráfico
	// form->RedrawTelaReal();
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::Redraw(int op, VTEdt *editor)
{
	// salva ponteiro para objeto
	this->editor = editor;
	// atualiza parte gráfica afetada pelo Editor
	switch (op)
	{
	case redrawFIG_SEL_COR_ORIG:  //figuras selecionadas, cor original
         RedrawFiguraSelCorOriginal();
         break;
	case redrawFIG_SEL_COR_UNICA:  //figuras selecionadas, cor única
         RedrawFiguraCorUnica();
         break;
	case redrawFIG_NOVA:  //figuras novas
         RedrawFiguraNova();
		 break;
	case redrawEQP_SEL_COR_ORIG: // equipamentos selecionados, cor original
		RedrawAllEqptoSelCorOriginal();
		break;
	case redrawEQP_SEL_COR_UNICA: // equipamentos selecionados, cor única
		RedrawAllEqptoSelCorUnica();
		break;
	case redrawEQP_NOVO: // equipamentos novos
		RedrawAllEqptoNovo();
		break;
	case redrawMARCA: // marcas
		RedrawAllMarca();
		break;
	case redrawREGIAO: // regiões
		RedrawRegiao();
		break;
	}
	// atualiza gráfico
	form->RedrawTelaReal();
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawEditor(void)
{
	// proteção
	if (editor == NULL)
		return;
	// apresenta equipamentos selecionados c/ cor única
	RedrawAllEqptoSelCorUnica();
	// apresenta marcas
	RedrawAllMarca();
	// apresenta equipamentos novos
	RedrawAllEqptoNovo();
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawAllEqptoSelCorOriginal(void)
{
	// variáveis locais
	TList *lisEQP;
	VTBarra *barra;
	VTEqpto *eqpto;
	VTLigacao *pliga;
	VTRede *rede;
	VTReducao *reducao;

	// loop p/ todos os eqptos selecionados
	if ((lisEQP = editor->LisEqptoSel()) == NULL)
		return;
	// atualiza todos Eqptos p/ serem desenhados
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Status[sttREDRAW] = true;
	}
	// loop p/ todos Eqptos
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (!eqpto->Status[sttREDRAW])
			continue;
		switch (eqpto->Tipo())
		{ // rede
		case eqptoREDE:
			rede = (VTRede*)eqpto;
			RedrawRedeEsquematico(rede, rede->Color);
			break;
			// reduçao
		case eqptoREDUCAO:
			reducao = (VTReducao*)eqpto;
			RedrawReducao(reducao, reducao->MRede->Color);
			break;
			// mutua
		case eqptoMUTUA:
			RedrawMutua((VTMutua*)eqpto, Color(eqpto, clWhite));
			break;
			// ligacao
		case eqptoCAPSERIE:
			RedrawLigacaoCorOriginal((VTLigacao*)eqpto);
			break;
		case eqptoCHAVE:
			RedrawLigacaoCorOriginal((VTLigacao*)eqpto);
			break;
		case eqptoREGULADOR:
			RedrawLigacaoCorOriginal((VTRegulador*)eqpto);
			break;
		case eqptoTRECHO:
			RedrawLigacaoCorOriginal((VTTrecho*)eqpto);
			break;
		case eqptoTRAFO:
			RedrawLigacaoCorOriginal((VTTrafo*)eqpto);
			break;
		case eqptoTRAFO3E:
			RedrawLigacaoCorOriginal((VTTrafo3E*)eqpto);
			break;
			// eqbar
		case eqptoCAPACITOR:
			RedrawCapacitor((VTCapacitor*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoCARGANL:
		case eqptoCARGA:
			RedrawCarga((VTCarga*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoFILTRO:
			RedrawFiltro((VTFiltro*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoGERADOR:
			RedrawGerador((VTGerador*)eqpto, Color(eqpto, clWhite));
			break;
			// case eqptoMEDIDOR:    RedrawMedidor((VTEqbar*)eqpto, Color(eqpto, padrao->med.cor));  break;
		case eqptoREATOR:
			RedrawReator((VTReator*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoSUPRIMENTO:
			RedrawSuprimento((VTSuprimento*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoTRAFOZZ:
			RedrawTrafoZZ((VTTrafoZZ*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoYREF:
			RedrawYref((VTYref*)eqpto, Color(eqpto, clWhite));
			break;
		case eqptoMOTOR:
			RedrawMotor((VTMotor*)eqpto, Color(eqpto, clWhite));
			break;
			// barra
		case eqptoBARRA:
			RedrawBarraCorOriginal((VTBarra*)eqpto);
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawAllEqptoSelCorUnica(void)
{
	// variáveis locais
	TList *lisEQP;
	VTEqpto *eqpto;

	// loop p/ todos os eqptos selecionados
	if ((lisEQP = editor->LisEqptoSel()) == NULL)
		return;
	// atualiza todos Eqptos p/ serem desenhados
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Status[sttREDRAW] = true;
	}
	// loop p/ todos Eqptos
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (!eqpto->Status[sttREDRAW])
			continue;
		switch (eqpto->Tipo())
		{ // barra
		case eqptoBARRA:
			RedrawBarra((VTBarra*)eqpto, cor.eqpto_sel);
			break;
			// eqbar
		case eqptoCAPACITOR:
			RedrawCapacitor((VTCapacitor*)eqpto, cor.eqpto_sel);
			break;
		case eqptoCARGANL:
		case eqptoCARGA:
			RedrawCarga((VTCarga*)eqpto, cor.eqpto_sel);
			break;
		case eqptoFILTRO:
			RedrawFiltro((VTFiltro*)eqpto, cor.eqpto_sel);
			break;
		case eqptoGERADOR:
			RedrawGerador((VTGerador*)eqpto, cor.eqpto_sel);
			break;
			// case eqptoMEDIDOR:    RedrawMedidor((VTEqbar*)eqpto, cor.eqpto_sel);       break;
		case eqptoREATOR:
			RedrawReator((VTReator*)eqpto, cor.eqpto_sel);
			break;
		case eqptoSUPRIMENTO:
			RedrawSuprimento((VTSuprimento*)eqpto, cor.eqpto_sel);
			break;
		case eqptoTRAFOZZ:
			RedrawTrafoZZ((VTTrafoZZ*)eqpto, cor.eqpto_sel);
			break;
		case eqptoYREF:
			RedrawYref((VTYref*)eqpto, cor.eqpto_sel);
			break;
		case eqptoMOTOR:
			RedrawMotor((VTMotor*)eqpto, cor.eqpto_sel);
			break;
			// ligacao
		case eqptoCAPSERIE:
			RedrawCapserie((VTCapserie*)eqpto, cor.eqpto_sel);
			break;
		case eqptoCHAVE:
			RedrawChave((VTChave*)eqpto, cor.eqpto_sel);
			break;
		case eqptoREGULADOR:
			RedrawRegulador((VTRegulador*)eqpto, cor.eqpto_sel);
			break;
		case eqptoTRECHO:
			RedrawTrecho((VTTrecho*)eqpto, cor.eqpto_sel);
			break;
		case eqptoTRAFO:
			RedrawTrafo((VTTrafo*)eqpto, cor.eqpto_sel);
			break;
		case eqptoTRAFO3E:
			RedrawTrafo3E((VTTrafo3E*)eqpto, cor.eqpto_sel);
			break;
			// mutua
		case eqptoMUTUA:
			RedrawMutua((VTMutua*)eqpto, cor.eqpto_sel);
			break;
			// rede
		case eqptoREDE:
			RedrawRedeEsquematico((VTRede*)eqpto, cor.eqpto_sel);
			break;
			// redução
		case eqptoREDUCAO:
			RedrawReducao((VTReducao*)eqpto, cor.eqpto_sel);
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawAllEqptoNovo(void)
{
	// variáveis locais
	TList *lisEQP;
	VTEqpto *eqpto;

	// loop p/ todos os eqptos selecionados
	if ((lisEQP = editor->LisEqptoNovo()) == NULL)
		return;
	// atualiza todos Eqptos p/ serem desenhados
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Status[sttREDRAW] = true;
	}
	// se o número de eqptos é muito grande, desenha apenas as Ligacoes
	if (lisEQP->Count < 200)
	{ // exibe p/ todos Eqptos
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			if (!eqpto->Status[sttREDRAW])
				continue;
			eqpto = (VTEqpto*)lisEQP->Items[n];
			draw->EqptoNovo(eqpto);
		}
	}
	else
	{ // exibe apenas as Ligacoes
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			if (!eqpto->Status[sttREDRAW])
				continue;
			if (eqpto->TipoLigacao())
			{
				draw->EqptoNovo(eqpto);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawAllMarca(void)
{
	// variáveis locais
	VTMarca *marca;
	TList *lisMAR;

	// obtém lista de Marcas
	if ((lisMAR = editor->LisMarca()) == NULL)
		return;
	for (int n = 0; n < lisMAR->Count; n++)
	{
		marca = (VTMarca*)lisMAR->Items[n];
		draw->Marca(marca);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16270997
void __fastcall TUnifEditor::RedrawBarraCorOriginal(VTBarra *barra)
{
	// variáveis locais
	int ind_cor;
	TColor cor;

	// verifica se é o gráfico de fluxo
	if (Tipo() == grafFLOW)
	{ // assume cor do diagnóstico do flow
		ind_cor = barra->resflow->Diagnostico[padrao->flow.num_pat];
		cor = geral->COR.Faixa[ind_cor];
	}
	else
	{ // verifica se é uma Barra isolada
		if (barra->Status[sttISOLADO])
		{ // assume cor de barra isolada
			cor = padrao->isolado.cor;
		}
		else if ((barra->Obra != NULL) && (barra->Obra->ColorEnabled))
		{ //verifica se os Eqptos da Obras devem ser mostrados com cor diferenciada
			cor = barra->Obra->Color;
		}
		else
		{ // verifica se a Barra possui Eqbar
			cor = (barra->NumeroDeEqbar() == 0) ? padrao->barra.cor : padrao->barra.cor_eqbar;
		}
	}
	RedrawBarra(barra, cor);
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TUnifEditor::RedrawFiguraSelCorOriginal(void)
   {
   //variáveis locais
   AnsiString   classname;
   TList        *lisFIG;
   VTFigura     *figura;
   VTFiguraComentario *comentario;
   VTLink       *link;
   VTRetangulo  *retangulo;

   //loop p/ todos os eqptos selecionados
   if ((lisFIG = editor->LisEqptoSel()) == NULL) return;
   //atualiza todos Eqptos p/ serem desenhados
   for (int n = 0; n < lisFIG->Count; n++)
      {
      figura = (VTFigura*)lisFIG->Items[n];
      switch(figura->TipoFigura)
         {
         case figCOMENTARIO:
            comentario = (VTFiguraComentario*)figura;
            draw->FigComentario(comentario, comentario->Retangulo->FundoCor);
            break;
         case figLINK:
            link = (VTLink*)figura;
            draw->FigLink(link, link->Comentario->Retangulo->BordaCor);
            break;
         case figRETANGULO:
            retangulo = (VTRetangulo*)figura;
            draw->FigRetangulo(retangulo, retangulo->FundoCor);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TUnifEditor::RedrawFiguraCorUnica(void)
   {
   //variáveis locais
   TList        *lisFIG;
   VTFiguraComentario *comentario;
   VTFigura     *figura;
   VTLink       *link;
   VTRetangulo  *retangulo;

   //loop p/ todos os eqptos selecionados
   if ((lisFIG = editor->LisEqptoSel()) == NULL) return;
   //atualiza todos Eqptos p/ serem desenhados
   for (int n = 0; n < lisFIG->Count; n++)
      {
      figura = (VTFigura*)lisFIG->Items[n];
      switch(figura->TipoFigura)
         {
         case figCOMENTARIO:
            comentario = (VTFiguraComentario*)figura;
            draw->FigComentario(comentario, cor.eqpto_sel);
            break;
         case figLINK:
            link = (VTLink*)figura;
            draw->FigLink(link, cor.eqpto_sel);
            break;
         case figRETANGULO:
            retangulo = (VTRetangulo*)figura;
            draw->FigRetangulo(retangulo, cor.eqpto_sel);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TUnifEditor::RedrawFiguraNova(void)
   {
   //variáveis locais
   AnsiString   classname;
   TList        *lisFIG;
   VTFigura     *figura;

   //loop p/ todos as Figuras selecionadas
   if ((lisFIG = editor->LisEqptoNovo()) == NULL) return;
   for (int n = 0; n < lisFIG->Count; n++)
      {
      figura = (VTFigura*)lisFIG->Items[n];
      draw->FigNova(figura);
      }
   }

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16270997
void __fastcall TUnifEditor::RedrawLigacaoCorOriginal(VTLigacao *pliga)
{
	// variáveis locais
	int ind_cor;
	TColor cor;
	VTChave *chave;

	// verifica se é o gráfico de fluxo
	if (Tipo() == grafFLOW)
	{ // assume cor do diagnóstico do flow
		ind_cor = pliga->resflow->Diagnostico[padrao->flow.num_pat];
		cor = geral->COR.Faixa[ind_cor];
	}
	else
	{ // verifica se a Ligacao é uma Chave
		if (pliga->Tipo() == eqptoCHAVE)
		{
			chave = (VTChave*)pliga;
			if (padrao->chave.op_cor == 0)
				cor = chave->TipoChave->Color;
			else if (chave->rede != NULL)
				cor = chave->rede->Color;
			else
				cor = padrao->isolado.cor;
			// proteção: verifica se cor selecionada é igual à cor de fundo
			if (cor == redegraf->CorFundo)
			{
				if (cor == clBlack)
					cor = clWhite;
				else
					cor = clBlack;
			}
		}

		if ((pliga->Obra != NULL) && (pliga->Obra->ColorEnabled))
		{ //verifica se os Eqptos da Obras devem ser mostrados com cor diferenciada
			cor = pliga->Obra->Color;
		}
		else
		{ // assume cor da rede ou de Eqpto isolado
			cor = (pliga->rede != NULL) ? pliga->rede->Color : padrao->isolado.cor;
		}
	}
	switch (pliga->Tipo())
	{
	case eqptoCAPSERIE:
		RedrawCapserie((VTCapserie*)pliga, Color(pliga, cor));
		break;
	case eqptoCHAVE:
		RedrawChave((VTChave*)pliga, Color(pliga, cor));
		break;
	case eqptoREGULADOR:
		RedrawRegulador((VTRegulador*)pliga, Color(pliga, cor));
		break;
	case eqptoTRAFO:
		RedrawTrafo((VTTrafo*)pliga, Color(pliga, cor));
		break;
	case eqptoTRAFO3E:
		RedrawTrafo3E((VTTrafo3E*)pliga, Color(pliga, cor));
		break;
	case eqptoTRECHO:
		RedrawTrecho((VTTrecho*)pliga, Color(pliga, cor));
		break;
	}
}

// ---------------------------------------------------------------------------
// eof
