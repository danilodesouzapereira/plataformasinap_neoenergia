//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUnifConfiab.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TFormGraf.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Confiabilidade\VTCfgGrafConfiab.h"
#include "..\Confiabilidade\VTConfiab.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifConfiab(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifConfiab(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifConfiab::TUnifConfiab() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifConfiab::TUnifConfiab(VTApl *apl, int tipo_graf, AnsiString nome)
                      :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawCurto(apl);
   }

//---------------------------------------------------------------------------
// TUnifConfiab::~TUnifConfiab() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifConfiab::~TUnifConfiab(void)
   {
   //nada a fazer
	}
//---------------------------------------------------------------------------
TColor __fastcall TUnifConfiab::CorBarraConfiab(VTBarra *pbarra)
	{
	//variaveis locais
	VTConfiab        *confiab = (VTConfiab*)       apl->GetObject(__classid(VTConfiab));
	VTCfgGrafConfiab *cfg     = (VTCfgGrafConfiab*) apl->GetObject(__classid(VTCfgGrafConfiab));
	TList           *lisCarga;
	RES_CONF        res;
	TColor			 cor = clGray;

	//proteção:
	if(pbarra->LisEqbar()->Count == 0) return (clWhite);

	//cria lista
	lisCarga = new TList();

	//obtem lista de carga do ponto
	pbarra->LisEqbar(lisCarga, eqptoCARGA);

	if(lisCarga->Count)
		if(confiab->AgregaRes(res, lisCarga)) //agrega indices
			cor = cfg->Cor(res.dec, res.fec, res.end, res.pmed_mw); //cor apropriada

	//destroi lista criada
	if(lisCarga) {delete lisCarga; lisCarga = NULL;}

	return (cor);
	}


//---------------------------------------------------------------------------
TColor __fastcall TUnifConfiab::CorBlocoConfiab(VTBloco *pbloco)
	{
	//variaveis locais
	VTConfiab        *confiab = (VTConfiab*)       apl->GetObject(__classid(VTConfiab));
	VTCfgGrafConfiab *cfg     = (VTCfgGrafConfiab*) apl->GetObject(__classid(VTCfgGrafConfiab));
	TColor   	     cor = clGray;
	RES_CONF         res;
	//TList            *lisCarga;

	//proteção:
	if(! pbloco)  return cor;
	if(! confiab) return cor;
	if(! cfg)     return cor;


	/*
	//<su> 2010.03.10 foi implementada a função que retorna direta indices por bloco

	//cria lista
	lisCarga = new TList();

	//obtem lista de carga do ponto
	pbloco->LisEqbar(lisCarga, eqptoCARGA);

	if(lisCarga->Count)
		if(confiab->AgregaRes(res, lisCarga)) //agrega indices
			cor = cfg->Cor(res.dec, res.fec, res.end, res.pmed_mw); //cor apropriada

	//destroi lista criada
	if(lisCarga) {delete lisCarga; lisCarga = NULL;}
	*/

	if(confiab->AgregaRes(res, pbloco)) //agrega indices
			cor = cfg->Cor(res.dec, res.fec, res.end, res.pmed_mw); //cor apropriada

	return ((TColor) cor);
	}
//---------------------------------------------------------------------------
void __fastcall TUnifConfiab::Redraw(VTEdt *editor)
	{
	//obtém objeto RedeCC
	//redeCFB = (VTRedeCFB*)apl->GetObject(__classid(VTRedeCFB));
	//desabilita animação
	animacao->Stop();
	//utiliza método da classe base
	TUnifEditor::Redraw(editor);
	//atualiza gráfico
	form->RedrawTelaReal();
	}

//-----------------------------------------------------------------------------
void __fastcall TUnifConfiab::Redraw(int op, VTEdt *editor)
   {
	//obtém objeto RedeCC
	///redeCFB = (VTRedeCFB*)apl->GetObject(__classid(VTRedeCFB));
	//utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifConfiab::RedrawAllBarra(void)
   {
   //variáveis locais
	//int       ind_cor;
	TColor    cor;
	VTBarra   *pbarra;
	TList     *lisBAR;
	TList     *lisBlocos;
	VTBloco   *bloco;
	VTConfiab *confiab = (VTConfiab*) apl->GetObject(__classid(VTConfiab));

	VTBlocos *blocos;
	if(confiab == NULL)
	  {
	  VTBlocos *blocos = (VTBlocos*) apl->GetObject(__classid(VTBlocos));
	  lisBlocos = blocos->LisBloco();
		for(int n = 0; n < lisBlocos->Count; n++)
			{
			bloco = (VTBloco*) lisBlocos->Items[n];
			//determina a cor do bloco
			cor = bloco->Color;
			//desenha as barras de acordo cor do bloco
			lisBAR = bloco->LisBarra();
			for (int nbar = 0; nbar < lisBAR->Count; nbar++)
				{
				pbarra  = (VTBarra*)lisBAR->Items[nbar];
				RedrawBarra(pbarra, cor);
				}
			}
	  return;
	  }

	//<su> 2010.02.04
	//determina lista de blocos
	if(confiab != NULL && confiab->Blocos != NULL)
		{
		lisBlocos = confiab->Blocos->LisBloco();
		for(int n = 0; n < lisBlocos->Count; n++)
			{
			bloco = (VTBloco*) lisBlocos->Items[n];
			//determina a cor do bloco
			cor = CorBlocoConfiab(bloco);
			//desenha as barras de acordo cor do bloco
			lisBAR = bloco->LisBarra();
			for (int nbar = 0; nbar < lisBAR->Count; nbar++)
				{
				pbarra  = (VTBarra*)lisBAR->Items[nbar];
				RedrawBarra(pbarra, cor);
				}
			}
		}

	//determina lista de Barras
	else if ((lisBAR = LisEqptoRede(eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
		for (int nbar = 0; nbar < lisBAR->Count; nbar++)
			{
			pbarra  = (VTBarra*)lisBAR->Items[nbar];
			//ind_cor = pbarra->resflow->Diagnostico[padrao->flow.num_pat];
			cor     = clWhite;
			//desenha de acordo com índices de confiabilidade
			//if(pbarra->LisEqbar()->Count) cor = CorBarraConfiab(pbarra);    			
			RedrawBarra(pbarra, cor);
			}
		}
	}

//-----------------------------------------------------------------------------
void __fastcall TUnifConfiab::RedrawAllLigacao(bool chave_enabled)
   {
   //variáveis locais
	//int        ind_cor;
	TColor      cor;
	VTLigacao  *ligacao;
   TList      *lisEQP;
	TList      *lisBlocos;
	VTBloco    *bloco;
	VTConfiab  *confiab = (VTConfiab*) apl->GetObject(__classid(VTConfiab));

	VTBlocos *blocos;
	if(confiab == NULL)
	  {
	  VTBlocos *blocos = (VTBlocos*) apl->GetObject(__classid(VTBlocos));
	  lisBlocos = blocos->LisBloco();
		for(int n = 0; n < lisBlocos->Count; n++)
			{
			bloco = (VTBloco*) lisBlocos->Items[n];
			//determina a cor do bloco
			cor = bloco->Color;
			//desenha as barras de acordo cor do bloco
			lisEQP = bloco->LisLigacao();
			for (int n = 0; n < lisEQP->Count; n++)
				{
				ligacao = (VTLigacao*)lisEQP->Items[n];
				draw->LinhasLigacao(ligacao, cor);
				}
			}
		return;
		}
	//determina lista de blocos
	if(confiab != NULL && confiab->Blocos != NULL)
		{
		lisBlocos = confiab->Blocos->LisBloco();
		for(int n = 0; n < lisBlocos->Count; n++)
			{
			bloco = (VTBloco*) lisBlocos->Items[n];
			//determina a cor do bloco
			cor = CorBlocoConfiab(bloco);
			//desenha as barras de acordo cor do bloco
			lisEQP = bloco->LisLigacao();
			for (int n = 0; n < lisEQP->Count; n++)
				{
				ligacao = (VTLigacao*)lisEQP->Items[n];
				draw->LinhasLigacao(ligacao, cor);
				}
			}
		}

	//determina lista de Capserie
	else if ((lisEQP = LisEqptoRede(eqptoLIGACAO)) != NULL)
      {//loop p/ todos Capserie
		for (int n = 0; n < lisEQP->Count; n++)
			{
			ligacao = (VTLigacao*)lisEQP->Items[n];
			//ind_cor = capserie->resflow->Diagnostico[padrao->flow.num_pat];
			cor     = clWhite;
			//determina a cor da barra e desenha a cor do trecho igual a da barra
			//if(ligacao->pbarra1)	                  cor = CorBarraConfiab(ligacao->pbarra1);
			//if(ligacao->pbarra2 && cor == clWhite) cor = CorBarraConfiab(ligacao->pbarra2);
			
			draw->LinhasLigacao(ligacao, cor);
			/*
			switch(ligacao->Tipo())
				{
				case eqptoCAPSERIE:  RedrawCapserie((VTCcapserie*)ligacao,  cor);  break;
				case eqptoCHAVE:     RedrawChave((VTChave*)ligacao,         cor);  break;
				case eqptoREGULADOR: RedrawRegualdor((VTRegulador*)ligacao, cor);  break;
				case TRAFO:          RedrawTrafo((VTTrafo*)ligacao,         cor);  break;
				case eqptoTRAFO3E:   RedrawTrafo3E((VTTrafo3E*)ligacao,     cor);  break;
				case eqptoTRECHO:    RedrawTrecho((VTTrecho*)ligacao,       cor);  break;
				}
			*/
			}
		}
	}

//-----------------------------------------------------------------------------
void __fastcall TUnifConfiab::RedrawAllEqbar(void)
	{

	}

//---------------------------------------------------------------------------
//eof

