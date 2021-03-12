// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormEliminaFusivel.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Topologia\VTTopologia.h>

#include "..\VTCalcIndCont.h"
#include "..\VTBuscaProt.h"

#include <PlataformaSinap\DLL_Inc\Radial.h>
#include "..\..\..\DLL_Inc\CalcIndCont.h"
#include <PlataformaSinap\DLL_Inc\Rede.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEliminaFusivel *FormEliminaFusivel;

// ---------------------------------------------------------------------------
__fastcall TFormEliminaFusivel::TFormEliminaFusivel(TComponent* Owner, VTApl* apl_owner,
	TWinControl *parent) : TForm(Owner)
{

	this->apl = apl_owner;
	calcIndCont = NULL;
	// Cria lista de chaves para fusao de blocos
	// lisChavesFunde = new TList();
	// lisChavesNA = new TList();
	// !!Pendencia!! alterar para funcao em calcIndCont para
	// identificar chaves que ligam carga a rede, hj baseado em código FU
	// IniciaLisChavesFunde(lisChavesFunde);

   buscaProt = NewObjBuscaProt(apl, false);
   buscaProt->Inicia();
   IniciaTipoProt(buscaProt->LisTipoEqProt);

}

// ---------------------------------------------------------------------------
__fastcall TFormEliminaFusivel::~TFormEliminaFusivel(void)
{
	if (calcIndCont)
	{
		delete calcIndCont;
		calcIndCont = NULL;
	}
   if(buscaProt)    {delete buscaProt; buscaProt = NULL;}
	// if(lisChavesFunde)    {delete lisChavesFunde; lisChavesFunde = NULL;}
	// if(lisChavesNA)    {delete lisChavesNA; lisChavesNA = NULL;}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::Button1Click(TObject *Sender)
{
	EliminaChaves();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::Button3Click(TObject *Sender)
{
	IniciaLisChavesNA();

}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::ActionArvoreProtExecute(TObject *Sender)
   {
   //cria buscaProt e inicia
	VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRadial *radial;
   VTPrimario *primario;
   TList *lisPrimario;
   AnsiString aux;
   VTChave   *chave;
   TTreeNode *node;
   TList     *lisAux;
   TList   *lisBlocoExt, *lisChavesExt;



	radial = DLL_NewObjRadial(apl);
	radial->Inicia(redes);
   lisPrimario = radial->LisPrimario();

   tvChvProt->Items->Clear();
   lisAux = new TList();
   lisBlocoExt = new TList();
   lisChavesExt = new TList();
   for(int np = 0; np < lisPrimario->Count; np++)
      {
      primario = (VTPrimario*)lisPrimario->Items[np];

      if(primario->LigacaoInicial->Tipo() != eqptoCHAVE)
         {
		 aux.sprintf("Problema no circuito %s", primario->Rede->Codigo.c_str());
         Aviso(aux);
         continue;
         }

      chave = (VTChave*)primario->LigacaoInicial;
      lisAux->Add(tvChvProt->Items->AddChildObject(NULL, chave->Codigo, chave));
      while(lisAux->Count > 0)
         {
         node = (TTreeNode*)lisAux->First();
         lisAux->Delete(0);

         chave = (VTChave*)node->Data;
         lisBlocoExt->Clear();
         lisChavesExt->Clear();
         buscaProt->ListaZonaProtJus(chave, lisBlocoExt, lisChavesExt);
         for(int nc = 0; nc < lisChavesExt->Count; nc++)
            {
            chave = (VTChave*)lisChavesExt->Items[nc];
            lisAux->Add(tvChvProt->Items->AddChildObject(node, chave->Codigo, chave));
            }
         }
      }







   delete radial;
   delete lisAux;
   delete lisBlocoExt;
   delete lisChavesExt;
   }
//---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::ActionImprimeResultadosExecute(TObject *Sender)

   {
   TTreeNode *node, *nodeMont;
   VTChave   *chave, *chaveMont;
   VTCarga   *carga;
   VTBarra   *barra;
   TList   *lisBlocoExt, *lisChavesExt;
   VTBloco *bloco;
   VTLigacao *ligacao;
   TList *lisLiga, *lisCargas, *lisBarra;
   double comprimentoKm;
   int    numCons;
   AnsiString arq;
   FILE *foutBarra;     //barras do bloco
   FILE *foutChaveVar;     //variáveis por chave
   FILE *foutChaveHie;     //hierarquia de chave
	VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));  //hkprov

   arq = path->DirTmp() + "\\barraProt.csv";
   foutBarra = fopen(arq.c_str(), "wt");
   arq = path->DirTmp() + "\\varProt.csv";
   foutChaveVar = fopen(arq.c_str(), "wt");
   arq = path->DirTmp() + "\\hierProt.csv";
   foutChaveHie = fopen(arq.c_str(), "wt");

   fprintf(foutBarra, "codChave, codBarra\n");
   fprintf(foutChaveVar, "codChave, comprimento, numCons\n");
   fprintf(foutChaveHie, "codChave, codChaveMont\n");

   lisBlocoExt = new TList();
   lisChavesExt = new TList();
   lisCargas = new TList();
   for(int nno = 0; nno < tvChvProt->Items->Count; nno++)
      {
      node = tvChvProt->Items->Item[nno];
      chave = (VTChave*)node->Data;

      lisBlocoExt->Clear();
      lisChavesExt->Clear();
      buscaProt->ListaZonaProtJus(chave, lisBlocoExt, lisChavesExt);
      //Calcula o comprimento e número de consumidores
      comprimentoKm = 0.0;
      numCons = 0;
      for(int nb = 0; nb < lisBlocoExt->Count; nb++)
         {
         bloco = (VTBloco*)lisBlocoExt->Items[nb];
         lisLiga = bloco->LisLigacao();
         for(int nl = 0; nl < lisLiga->Count; nl++)
            {
            ligacao = (VTLigacao*)lisLiga->Items[nl];
            if(ligacao->Tipo() != eqptoTRECHO) continue;
            comprimentoKm += ((VTTrecho*)ligacao)->Comprimento_km;
            }

         lisCargas->Clear();
         bloco->LisEqbar(lisCargas, eqptoCARGA);
         for(int nc = 0; nc < lisCargas->Count; nc++)
            {
            carga = (VTCarga*)lisCargas->Items[nc];
            numCons += carga->NumConsTotal;
            }

         //Imprime as barras do bloco
         lisBarra = bloco->LisBarra();
         for(int nbar = 0; nbar < lisBarra->Count; nbar++)
            {
            barra = (VTBarra*)lisBarra->Items[nbar];
			fprintf(foutBarra, "%s, %s\n", chave->Codigo.c_str(), barra->Codigo.c_str());
            }
         }
      //Imprime as variáveis por chave
	  fprintf(foutChaveVar, "%s, %7.3f, %i\n", chave->Codigo.c_str(), comprimentoKm, numCons);

      //Imprime hierarquia de chave
      if((nodeMont = node->Parent) != NULL)
        {
        chaveMont = (VTChave*)nodeMont->Data;
        fprintf(foutChaveHie, "%s, %s\n", chave->Codigo.c_str(), chaveMont->Codigo.c_str());
        }
      }

   delete lisCargas;
   delete lisBlocoExt;
   delete lisChavesExt;

   fclose(foutBarra);     //barras do bloco
   fclose(foutChaveVar);  //variáveis por chave
   fclose(foutChaveHie);  //hierarquia de chave
   }
//---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::ActionMostraRamoArvoreExecute(TObject *Sender)
   {
   VTGrafico      *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TTreeNode *node;
   VTChave   *chave;
   TList   *lisBlocoExt, *lisChavesExt;
   TColor color;
   VTBloco *bloco;
   TList *lisLiga;

   if((node = tvChvProt->Selected) == NULL) return;

   lisBlocoExt = new TList();
   lisChavesExt = new TList();
   chave = (VTChave*)node->Data;
   lisBlocoExt->Clear();
   lisChavesExt->Clear();
   buscaProt->ListaZonaProtJus(chave, lisBlocoExt, lisChavesExt);

   lisLiga = new TList();
   for(int nb = 0; nb < lisBlocoExt->Count; nb++)
      {
      bloco = (VTBloco*)lisBlocoExt->Items[nb];
      if(bloco->Rede) color = bloco->Rede->Color;
      lisLiga->Assign(bloco->LisLigacao(), laOr);
      }


   lisChavesExt->Add(chave);
   grafico->Moldura(lisChavesExt);
   grafico->DestacaEqpto(lisLiga, color, 10);

   delete lisLiga;
   delete lisBlocoExt;
   delete lisChavesExt;
   }
//---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::IniciaTipoProt(TList  *lisTipoEqProt) //hk18
   {
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
   VTTipoChave *tipoChave;
   VTChave     *chave;
   TList       *lisChave;

   lisTipoEqProt->Clear();
   if(tipos == NULL)
     {
     //Popula a partir da lista de chaves

     lisChave = new TList();
     redes->LisEqpto(lisChave, eqptoCHAVE);
     for(int n = 0; n < lisChave->Count; n++)
        {
        chave = (VTChave*)lisChave->Items[n];
        tipoChave = chave->TipoChave;
        if(lisTipoEqProt->IndexOf(tipoChave) == -1) lisTipoEqProt->Add(tipoChave);
        }
     delete lisChave;
     }
   else
     {
     //Popula a partir de tipos

     lisTipoEqProt->Assign(tipos->LisTipoChave(), laOr);
     }

   for(int n = lisTipoEqProt->Count - 1; n--; n >= 0)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];
      if(tipoChave->Tipo ==chaveDJ) continue;
      if(tipoChave->Tipo ==chaveRA) continue;
      if(tipoChave->Tipo ==chaveBF) continue;
      lisTipoEqProt->Remove(tipoChave);
      }
   /*
   chaveDJ=1, chaveDISJUNTOR   =chaveDJ,
   chaveRA,   chaveRELIGADORA  =chaveRA,
   chaveBF,   chaveBASE_FUSIVEL=chaveBF,
   chaveSEC,  chaveSECCIONADORA=chaveSEC,
   chaveINDEFINIDA,
   chaveDEFINIDA_USUARIO,
   //chaves Eletropaulo/Subterrâneo
   chaveDESCONECTAVEL=1000, chaveNETWORK_PROTECTOR
   */

   for(int n = lisTipoEqProt->Count - 1; n--; n >= 0)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProt->Items[n];

      if(tipoChave->Codigo.AnsiCompare("Disjuntor") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Religador") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Base Fusível") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Chave Fusivel Repetidora MT") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Seccionalizador") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Chave Fusivel") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Chave Interruptora Tripolar Seca") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Chave Fusivel Especial 34,5kV") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Chave de Protecao Subterranea") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Posto Transformador 34,5kV com eqpto de proteçao/manobra") == 0) continue;
      if(tipoChave->Codigo.AnsiCompare("Posto Transformador 34,5kV sem eqpto de proteçao/manobra") == 0) continue;
      lisTipoEqProt->Remove(tipoChave);
      }
//      tipoChave->Codigo.AnsiCompare("Seccionadora") == 0) continue;
//      if(tipoChave->Codigo.AnsiCompare("Regulador de Tensao") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Faca") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Indefinida") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Autotransformador") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Basculante") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Banco de Capacitores") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Seccionadora MT SF6") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Unipolar") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Adaptada") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave Faca Interligaçao 13,8kV/34,5kV") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Seccionamento Operativo") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Chave de Manobra Subterranea") == 0) continue;
//      tipoChave->Codigo.AnsiCompare("Seccionamento de Primario") == 0) continue;

   }
//---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::ActionAvaliaDefeitoExecute(TObject *Sender)
   {
   //
   }
//---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::butEliminaNADuplicClick(TObject *Sender)
{
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTChave *chave, *chave1;
	TList *lisChavesNADupli, *lisChavesNA;
	AnsiString aux;
	// Cria lista de chaves NA duplicadas
	lisChavesNA = new TList();
	lisChavesNADupli = new TList();

	redes->LisEqpto(lisChavesNA, eqptoCHAVE);
	for (int nc = lisChavesNA->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChavesNA->Items[nc];
		if (chave->Aberta)
			continue;
		lisChavesNA->Delete(nc);
	}

	rchRes->Lines->Clear();
	rchRes->Lines->Add("Chaves NA da rede");
	rchRes->Lines->Add("chave\t barra1\t barra2");
	for (int nc = 0; nc < lisChavesNA->Count; nc++)
	{
		chave = (VTChave*)lisChavesNA->Items[nc];
		aux.sprintf("%s\t %s \t %s", chave->Codigo.c_str(), chave->pbarra1->Codigo.c_str(), chave->pbarra2->Codigo.c_str());
		rchRes->Lines->Add(aux);
	}
	rchRes->Lines->Add("-------------------");

	rchRes->Lines->Add("Chaves NA repetidas ");
	rchRes->Lines->Add("chave\t barra1\t barra2");
	for (int nc = 0; nc < lisChavesNA->Count; nc++)
	{
		chave = (VTChave*)lisChavesNA->Items[nc];
		chave1 = NULL;
		for (int nc1 = nc + 1; nc1 < lisChavesNA->Count; nc1++)
		{
			chave1 = (VTChave*)lisChavesNA->Items[nc1];
			if ((chave->pbarra1 == chave1->pbarra1) && (chave->pbarra2 == chave1->pbarra2))
				break;
			if ((chave->pbarra1 == chave1->pbarra2) && (chave->pbarra2 == chave1->pbarra1))
				break;
			chave1 = NULL;
		}
		if (chave1 != NULL)
		{
			aux.sprintf("%s\t %s \t %s", chave1->Codigo.c_str(), chave1->pbarra1->Codigo.c_str(),
				chave1->pbarra2->Codigo.c_str());
			lisChavesNADupli->Add(chave1);
			rchRes->Lines->Add(aux);
		}
	}
	rchRes->Lines->Add("-------------------");

	edita->RetiraLisEqpto(lisChavesNADupli);
	topologia->Reconfigura();

	delete lisChavesNADupli;
	delete lisChavesNA;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::IniciaLisChavesNA(void)
{
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
	VTChave *chaveNA;

	AnsiString Chave;
	AnsiString TIPO;
	AnsiString Circuito_1;
	AnsiString Circuito_2;
	AnsiString Barra_1;
	AnsiString Barra_2;
	// InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2);
	// Busca barra1 da rede

	rchRes->Lines->Clear();
	// Chave="F00694";TIPO="Não Definida";Circuito_1="AGS-01Z1";Circuito_2="AGS-01Z2";Barra_1="X158166";Barra_2="X158166";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
	// Chave="F00678";TIPO="Não Definida";Circuito_1="AGS-01Z1";Circuito_2="AGS-01Z5";Barra_1="X125863";Barra_2="X125863";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
	// Chave="F00681";TIPO="Não Definida";Circuito_1="AGS-01Z3";Circuito_2="AGS-01Z2";Barra_1="X158158";Barra_2="X158158";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
	// Chave="F00274";TIPO="Não Definida";Circuito_1="AGS-01Z4";Circuito_2="AGS-01Z3";Barra_1="X151587";Barra_2="X151587";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
	// Chave="F00693";TIPO="Não Definida";Circuito_1="AGS-01Z5";Circuito_2="AGS-01Z4";Barra_1="X457113";Barra_2="X457113";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);

	Chave = "F00694";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z1";
	Circuito_2 = "AGS-01Z2";
	Barra_1 = "X158166";
	Barra_2 = "X158166";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00678";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z1";
	Circuito_2 = "AGS-01Z5";
	Barra_1 = "X125863";
	Barra_2 = "X125863";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00681";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z3";
	Circuito_2 = "AGS-01Z2";
	Barra_1 = "X158158";
	Barra_2 = "X158158";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00274";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z4";
	Circuito_2 = "AGS-01Z3";
	Barra_1 = "X151587";
	Barra_2 = "X151587";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00693";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z5";
	Circuito_2 = "AGS-01Z4";
	Barra_1 = "X457113";
	Barra_2 = "X457113";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00241";
	TIPO = "Não Definida";
	Circuito_1 = "AGS-01Z5";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "8715757-1";
	Barra_2 = "8715757-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00144";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S1";
	Circuito_2 = "AML-01X1";
	Barra_1 = "X055184";
	Barra_2 = "X055184";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00357";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S1";
	Circuito_2 = "AML-01X4";
	Barra_1 = "X062138";
	Barra_2 = "X062138";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00449";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S1";
	Circuito_2 = "AML-01X2";
	Barra_1 = "X501463";
	Barra_2 = "X501463";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00336";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S1";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X501464";
	Barra_2 = "X501464";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57147";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S1";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "12893532-1";
	Barra_2 = "12893532-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F28193";
	TIPO = "Chave Fusível";
	Circuito_1 = "AML-01S2";
	Circuito_2 = "AML-01S2";
	Barra_1 = "12761406-1";
	Barra_2 = "12761406-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26253";
	TIPO = "Seccionalizadora";
	Circuito_1 = "AML-01S2";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X055710";
	Barra_2 = "118681-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26232";
	TIPO = "Seccionalizadora";
	Circuito_1 = "AML-01S2";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X055722";
	Barra_2 = "118493-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00048";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S2";
	Circuito_2 = "PIT-01M2";
	Barra_1 = "104341197-1";
	Barra_2 = "104341197-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00518";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S3";
	Circuito_2 = "PIT-01M2";
	Barra_1 = "9655097-1";
	Barra_2 = "9655097-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F14988";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S3";
	Circuito_2 = "PIT-01M2";
	Barra_1 = "X023102";
	Barra_2 = "X023102";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53381";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S3";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "X023111";
	Barra_2 = "X023111";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00214";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S4";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X054585";
	Barra_2 = "X054585";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00715";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S4";
	Circuito_2 = "AML-01X4";
	Barra_1 = "X460252";
	Barra_2 = "X460252";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26251";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S5";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X022809";
	Barra_2 = "X022809";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00247";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S5";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X022772";
	Barra_2 = "9655083-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00526";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01S5";
	Circuito_2 = "PIT-01Y5";
	Barra_1 = "X022722";
	Barra_2 = "X022722";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F09971";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X1";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X055801";
	Barra_2 = "X055801";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00445";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X2";
	Circuito_2 = "AML-01X1";
	Barra_1 = "9622915-1";
	Barra_2 = "9622915-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F23389";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X2";
	Circuito_2 = "AML-01X3";
	Barra_1 = "X059380";
	Barra_2 = "X059380";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00520";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X2";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "9627007-1";
	Barra_2 = "9627007-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00084";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X2";
	Circuito_2 = "FDR-01Z3";
	Barra_1 = "X117838";
	Barra_2 = "X117838";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00521";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X2";
	Circuito_2 = "FDR-01J1";
	Barra_1 = "X112372";
	Barra_2 = "X112372";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50140";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X3";
	Circuito_2 = "AML-01X1";
	Barra_1 = "X499660";
	Barra_2 = "X499660";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00197";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X4";
	Circuito_2 = "AML-01S5";
	Barra_1 = "X025232";
	Barra_2 = "X025232";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00532";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X4";
	Circuito_2 = "CDL-01P7";
	Barra_1 = "X054980";
	Barra_2 = "X054980";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50369";
	TIPO = "Não Definida";
	Circuito_1 = "AML-01X4";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X500433";
	Barra_2 = "X500433";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54801";
	TIPO = "Não Definida";
	Circuito_1 = "ARB-01V1";
	Circuito_2 = "ARB-01V2";
	Barra_1 = "12732599-1";
	Barra_2 = "12732599-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53047";
	TIPO = "Não Definida";
	Circuito_1 = "ARB-01V2";
	Circuito_2 = "LDF-01M4";
	Barra_1 = "X034017";
	Barra_2 = "X034017";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F14082";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C1";
	Circuito_2 = "CAB-01C4";
	Barra_1 = "104725643-1";
	Barra_2 = "104725643-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00319";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C1";
	Circuito_2 = "CAB-01C4";
	Barra_1 = "X607661";
	Barra_2 = "X607661";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00548";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C1";
	Circuito_2 = "PUD-01L6";
	Barra_1 = "12325352-1";
	Barra_2 = "12325352-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50452";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C2";
	Circuito_2 = "CAB-01C1";
	Barra_1 = "X008716";
	Barra_2 = "X008716";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00652";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C2";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "X487618";
	Barra_2 = "X487618";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51996";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C2";
	Circuito_2 = "PUD-01P2";
	Barra_1 = "X008598";
	Barra_2 = "X008598";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00229";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C3";
	Circuito_2 = "CAB-01C2";
	Barra_1 = "X602378";
	Barra_2 = "X602378";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F23473";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C4";
	Circuito_2 = "CAB-01C6";
	Barra_1 = "X001769";
	Barra_2 = "X001769";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F09132";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C4";
	Circuito_2 = "CAB-01C6";
	Barra_1 = "104725541-1";
	Barra_2 = "104725541-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00410";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C4";
	Circuito_2 = "CAB-01C6";
	Barra_1 = "8363440-1";
	Barra_2 = "8363440-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00320";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C5";
	Circuito_2 = "CAB-01C7";
	Barra_1 = "9173181-1";
	Barra_2 = "9173181-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00193";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C5";
	Circuito_2 = "CAB-01S2";
	Barra_1 = "X002346";
	Barra_2 = "X002346";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57008";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C6";
	Circuito_2 = "CAB-01S3";
	Barra_1 = "X001873";
	Barra_2 = "X001873";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00255";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C6";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "108636-1_AUX2";
	Barra_2 = "X002048";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00310";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C7";
	Circuito_2 = "CAB-01C6";
	Barra_1 = "X001825";
	Barra_2 = "X001825";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F55115";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C7";
	Circuito_2 = "MTT-01L6";
	Barra_1 = "X490922";
	Barra_2 = "X490922";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00396";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01C8";
	Circuito_2 = "CAB-01C7";
	Barra_1 = "X000454";
	Barra_2 = "X000454";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00064";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01S1";
	Circuito_2 = "CAB-01C8";
	Barra_1 = "X006640";
	Barra_2 = "X006640";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00289";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01S1";
	Circuito_2 = "CJD-01F6";
	Barra_1 = "X154934";
	Barra_2 = "X154934";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00090";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01S1";
	Circuito_2 = "RET-01F4";
	Barra_1 = "X014274";
	Barra_2 = "X014274";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00429";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01S1";
	Circuito_2 = "RET-01F4";
	Barra_1 = "X155516";
	Barra_2 = "X155516";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00116";
	TIPO = "Não Definida";
	Circuito_1 = "CAB-01S2";
	Circuito_2 = "RET-01F1";
	Barra_1 = "X003056";
	Barra_2 = "9309101-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00296";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P1";
	Circuito_2 = "CAJ-01P2";
	Barra_1 = "X148164";
	Barra_2 = "X148164";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49991";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P1";
	Circuito_2 = "LBT-01S2";
	Barra_1 = "X140702";
	Barra_2 = "X140702";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00148";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P2";
	Circuito_2 = "CAJ-01P1";
	Barra_1 = "X147492";
	Barra_2 = "X147492";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50996";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P2";
	Circuito_2 = "CAJ-01P3";
	Barra_1 = "X149623";
	Barra_2 = "X149623";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00659";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P2";
	Circuito_2 = "AGS-01Z3";
	Barra_1 = "104393600-1";
	Barra_2 = "104393600-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00249";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P2";
	Circuito_2 = "CJD-01F1";
	Barra_1 = "X124146";
	Barra_2 = "X124146";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F10798";
	TIPO = "Chave Fusível";
	Circuito_1 = "CAJ-01P3";
	Circuito_2 = "CAJ-01P3";
	Barra_1 = "X149616";
	Barra_2 = "4614-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00669";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P3";
	Circuito_2 = "AGS-01Z4";
	Barra_1 = "X150702";
	Barra_2 = "X150710";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00682";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P3";
	Circuito_2 = "AGS-01Z3";
	Barra_1 = "X158868";
	Barra_2 = "X158868";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51035";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CAJ-01P4";
	Circuito_2 = "CAJ-01P4";
	Barra_1 = "X704813";
	Barra_2 = "12473868-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00668";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P4";
	Circuito_2 = "CAJ-01P3";
	Barra_1 = "X124222";
	Barra_2 = "X150131";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52810";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P4";
	Circuito_2 = "SCR-01N8";
	Barra_1 = "X149318";
	Barra_2 = "X149318";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50579";
	TIPO = "Não Definida";
	Circuito_1 = "CAJ-01P4";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "X700225";
	Barra_2 = "X700225";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00206";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P1";
	Circuito_2 = "CDL-01P5";
	Barra_1 = "X066178";
	Barra_2 = "X066178";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00355";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P1";
	Circuito_2 = "CDL-01P6";
	Barra_1 = "X453638";
	Barra_2 = "X453638";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00272";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P1";
	Circuito_2 = "CDL-01W1";
	Barra_1 = "X465343";
	Barra_2 = "X465343";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26992";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P1";
	Circuito_2 = "CDL-01W1";
	Barra_1 = "104067876-1";
	Barra_2 = "104067876-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26852";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P1";
	Circuito_2 = "CDL-01W4";
	Barra_1 = "X501371";
	Barra_2 = "X501371";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26850";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P2";
	Circuito_2 = "CDL-01W4";
	Barra_1 = "104725841-1";
	Barra_2 = "104725841-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00046";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P4";
	Circuito_2 = "CDL-01P3";
	Barra_1 = "7192519-1";
	Barra_2 = "7192519-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00073";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P4";
	Circuito_2 = "MTT-01L4";
	Barra_1 = "9465307-1";
	Barra_2 = "9465307-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00019";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P5";
	Circuito_2 = "CDL-01W2";
	Barra_1 = "9556626-1";
	Barra_2 = "9556626-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F32810";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P6";
	Circuito_2 = "MTT-01L5";
	Barra_1 = "X415198";
	Barra_2 = "X415198";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00098";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P6";
	Circuito_2 = "MTT-01L5";
	Barra_1 = "12536671-1";
	Barra_2 = "12536671-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00023";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P7";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X053912";
	Barra_2 = "X053912";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00335";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P7";
	Circuito_2 = "CDL-01P3";
	Barra_1 = "104101839-1";
	Barra_2 = "104101839-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26544";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P7";
	Circuito_2 = "CDL-01W3";
	Barra_1 = "116979-1";
	Barra_2 = "X032820";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F36532";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P7";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X202457";
	Barra_2 = "X202457";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00198";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P8";
	Circuito_2 = "CDL-01P3";
	Barra_1 = "X200381";
	Barra_2 = "X200381";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50255";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P8";
	Circuito_2 = "CDL-01W3";
	Barra_1 = "X064937";
	Barra_2 = "X064937";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00383";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P8";
	Circuito_2 = "PIT-01Y3";
	Barra_1 = "9601422-1";
	Barra_2 = "9601422-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00155";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P8";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X032842";
	Barra_2 = "X032842";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F20667";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01P8";
	Circuito_2 = "PIT-01Y5";
	Barra_1 = "X032808";
	Barra_2 = "X032808";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00133";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W1";
	Circuito_2 = "CDL-01P1";
	Barra_1 = "X453640";
	Barra_2 = "X453640";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00360";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W2";
	Circuito_2 = "CDL-01P8";
	Barra_1 = "X065522";
	Barra_2 = "X065522";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00126";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W3";
	Circuito_2 = "CDL-01P3";
	Barra_1 = "7526648-1";
	Barra_2 = "7526648-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F27265";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W3";
	Circuito_2 = "CDL-01P3";
	Barra_1 = "X099913";
	Barra_2 = "X099913";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00675";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W5";
	Circuito_2 = "CDL-01W1";
	Barra_1 = "X453641";
	Barra_2 = "X453641";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26732";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W5";
	Circuito_2 = "PIT-01Y8";
	Barra_1 = "104068675-1";
	Barra_2 = "104068675-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00021";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W5";
	Circuito_2 = "PIT-01Y8";
	Barra_1 = "X032795";
	Barra_2 = "X032795";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51958";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W7";
	Circuito_2 = "CAB-01S3";
	Barra_1 = "X613658";
	Barra_2 = "X613658";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49337";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W7";
	Circuito_2 = "CDL-01W5";
	Barra_1 = "X001406";
	Barra_2 = "X001406";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00153";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W7";
	Circuito_2 = "CAB-01C7";
	Barra_1 = "X001432";
	Barra_2 = "X001432";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00374";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W7";
	Circuito_2 = "IMB-01J1";
	Barra_1 = "X010014";
	Barra_2 = "X010014";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00156";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W8";
	Circuito_2 = "CDL-01P6";
	Barra_1 = "X102859";
	Barra_2 = "X102859";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00535";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W8";
	Circuito_2 = "MTT-01L6";
	Barra_1 = "X102854";
	Barra_2 = "X102854";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00712";
	TIPO = "Não Definida";
	Circuito_1 = "CDL-01W8";
	Circuito_2 = "MTT-01L4";
	Barra_1 = "X103136";
	Barra_2 = "X103136";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04226";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01J4";
	Circuito_2 = "CEN-01P1";
	Barra_1 = "104159129-1";
	Barra_2 = "104159129-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F05727";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01J4";
	Circuito_2 = "FDR-01Z4";
	Barra_1 = "X117788";
	Barra_2 = "X117788";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00124";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01J5";
	Circuito_2 = "CEN-01M1";
	Barra_1 = "X701052";
	Barra_2 = "X701052";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00245";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01J5";
	Circuito_2 = "FNV-01V2";
	Barra_1 = "X111614";
	Barra_2 = "X111614";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00390";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M1";
	Circuito_2 = "CEN-01X4";
	Barra_1 = "X701776";
	Barra_2 = "X701776";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F06349";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CEN-01M2";
	Circuito_2 = "CEN-01M2";
	Barra_1 = "X701642";
	Barra_2 = "164496-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00040";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M2";
	Circuito_2 = "CEN-01M3";
	Barra_1 = "X701439";
	Barra_2 = "X701439";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00078";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M2";
	Circuito_2 = "CEN-01X4";
	Barra_1 = "X702086";
	Barra_2 = "X702086";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F16405";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M2";
	Circuito_2 = "CEN-01J4";
	Barra_1 = "X073755";
	Barra_2 = "X073755";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00044";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M3";
	Circuito_2 = "FNV-01V2";
	Barra_1 = "X111709";
	Barra_2 = "X111709";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00352";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01M3";
	Circuito_2 = "MTT-01N5";
	Barra_1 = "X111123";
	Barra_2 = "X111123";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50588";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01P1";
	Circuito_2 = "CEN-01P2";
	Barra_1 = "12712853-1";
	Barra_2 = "12712853-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50584";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01P1";
	Circuito_2 = "CEN-01P2";
	Barra_1 = "12712876-1";
	Barra_2 = "12712876-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50576";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01P1";
	Circuito_2 = "CEN-01P2";
	Barra_1 = "12714546-1";
	Barra_2 = "12714546-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F48323";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01P3";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "12662812-1";
	Barra_2 = "12662812-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34797";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CEN-01X1";
	Circuito_2 = "CEN-01X1";
	Barra_1 = "12152658-1";
	Barra_2 = "12152658-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04405";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X1";
	Circuito_2 = "CEN-01J1";
	Barra_1 = "X702317";
	Barra_2 = "X702317";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F18687";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X1";
	Circuito_2 = "LPN-01M1";
	Barra_1 = "X119916";
	Barra_2 = "X119916";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50330";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X2";
	Circuito_2 = "CEN-01X1";
	Barra_1 = "12722380-1";
	Barra_2 = "12722380-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50336";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X2";
	Circuito_2 = "CEN-01X1";
	Barra_1 = "12740373-1";
	Barra_2 = "12740373-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50832";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CEN-01X4";
	Circuito_2 = "CEN-01X4";
	Barra_1 = "X703538";
	Barra_2 = "12439805-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "C50586";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X5";
	Circuito_2 = "CEN-01X2";
	Barra_1 = "12722370-1";
	Barra_2 = "12722370-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04395";
	TIPO = "Não Definida";
	Circuito_1 = "CEN-01X5";
	Circuito_2 = "LPN-01M2";
	Barra_1 = "12662837-1";
	Barra_2 = "12662837-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00480";
	TIPO = "Não Definida";
	Circuito_1 = "CID-01V1";
	Circuito_2 = "CID-01V2";
	Barra_1 = "X270283";
	Barra_2 = "X270283";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90050";
	TIPO = "Seccionalizadora Tripolar";
	Circuito_1 = "CID-01V3";
	Circuito_2 = "CID-01V3";
	Barra_1 = "12537920-1";
	Barra_2 = "12537920-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00497";
	TIPO = "Não Definida";
	Circuito_1 = "CID-01V3";
	Circuito_2 = "CID-01V1";
	Barra_1 = "X273081";
	Barra_2 = "X273081";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00498";
	TIPO = "Não Definida";
	Circuito_1 = "CID-01V3";
	Circuito_2 = "MTR-01X1";
	Barra_1 = "X090304";
	Barra_2 = "X090304";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00591";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y1";
	Circuito_2 = "CIU-01V3";
	Barra_1 = "9685442-2";
	Barra_2 = "9685442-2";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F47649";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y1";
	Circuito_2 = "CIU-01V2";
	Barra_1 = "X163326";
	Barra_2 = "X163326";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00258";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y1";
	Circuito_2 = "CIU-01V4";
	Barra_1 = "931620-1";
	Barra_2 = "931620-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00271";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y1";
	Circuito_2 = "CIU-01W4";
	Barra_1 = "X466063";
	Barra_2 = "X466063";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00344";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y2";
	Circuito_2 = "CIT-01Y3";
	Barra_1 = "X198346";
	Barra_2 = "X198346";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00719";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y2";
	Circuito_2 = "CIT-01Y1";
	Barra_1 = "X146148";
	Barra_2 = "X146148";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00679";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y2";
	Circuito_2 = "IIN-01F4";
	Barra_1 = "X168280";
	Barra_2 = "X168280";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00269";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y3";
	Circuito_2 = "CIU-01V5";
	Barra_1 = "X488691";
	Barra_2 = "X488691";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00262";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y3";
	Circuito_2 = "CIU-01V6";
	Barra_1 = "X146158";
	Barra_2 = "X146158";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51179";
	TIPO = "Chave a óleo";
	Circuito_1 = "CIT-01Y4";
	Circuito_2 = "CIT-01Y4";
	Barra_1 = "X700363";
	Barra_2 = "12484566-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56439";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y4";
	Circuito_2 = "CIT-01Y6";
	Barra_1 = "X146856";
	Barra_2 = "X146856";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00697";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y4";
	Circuito_2 = "CIU-01V3";
	Barra_1 = "X160246";
	Barra_2 = "X160246";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00574";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y4";
	Circuito_2 = "CIU-01V5";
	Barra_1 = "X143812";
	Barra_2 = "X143812";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56738";
	TIPO = "Chave a óleo";
	Circuito_1 = "CIT-01Y6";
	Circuito_2 = "CIT-01Y6";
	Barra_1 = "X608480";
	Barra_2 = "12756458-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56349";
	TIPO = "Não Definida";
	Circuito_1 = "CIT-01Y6";
	Circuito_2 = "CIT-01Y4";
	Barra_1 = "X608507";
	Barra_2 = "X608507";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00347";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V1";
	Circuito_2 = "CIU-01W1";
	Barra_1 = "X079133";
	Barra_2 = "X079133";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90904";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V2";
	Circuito_2 = "CIT-01Y2";
	Barra_1 = "12794458-1";
	Barra_2 = "12794458-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00324";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V2";
	Circuito_2 = "CIU-01W3";
	Barra_1 = "X162999";
	Barra_2 = "X162999";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90502";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V3";
	Circuito_2 = "CIT-01Y4";
	Barra_1 = "X700362";
	Barra_2 = "X700362";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00642";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V3";
	Circuito_2 = "CIU-01V1";
	Barra_1 = "X162409";
	Barra_2 = "X162409";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00350";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V3";
	Circuito_2 = "CIU-01W4";
	Barra_1 = "104916009-1";
	Barra_2 = "104916009-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00295";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V4";
	Circuito_2 = "CIU-01W1";
	Barra_1 = "D163529X";
	Barra_2 = "D163529X";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00281";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V5";
	Circuito_2 = "CIU-01V6";
	Barra_1 = "X400870";
	Barra_2 = "X198339";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00035";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V6";
	Circuito_2 = "CIT-01Y1";
	Barra_1 = "X466877";
	Barra_2 = "X466877";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00020";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V6";
	Circuito_2 = "CIU-01V2";
	Barra_1 = "12463806-1";
	Barra_2 = "12463806-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00349";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V6";
	Circuito_2 = "CIU-01V3";
	Barra_1 = "X198337";
	Barra_2 = "X198337";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F17645";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V7";
	Circuito_2 = "CIU-01V8";
	Barra_1 = "104725540-1";
	Barra_2 = "104725540-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00732";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V7";
	Circuito_2 = "CIU-01V4";
	Barra_1 = "X163485";
	Barra_2 = "X163485";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90911";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V7";
	Circuito_2 = "CIU-01V8";
	Barra_1 = "12769801-1";
	Barra_2 = "12769801-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00337";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V8";
	Circuito_2 = "CIU-01V7";
	Barra_1 = "X163465";
	Barra_2 = "X163465";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00093";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V8";
	Circuito_2 = "CIU-01V7";
	Barra_1 = "X163407";
	Barra_2 = "X163407";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00292";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01V8";
	Circuito_2 = "PPE-01Y1";
	Barra_1 = "X163682";
	Barra_2 = "X163682";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00250";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01W1";
	Circuito_2 = "CIU-01V4";
	Barra_1 = "X152952";
	Barra_2 = "X152952";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00749";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01W2";
	Circuito_2 = "CIU-01W1";
	Barra_1 = "X468003";
	Barra_2 = "X468003";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00286";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01W2";
	Circuito_2 = "PPE-01Y4";
	Barra_1 = "X078065";
	Barra_2 = "X078065";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00348";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01W3";
	Circuito_2 = "CIU-01V1";
	Barra_1 = "X163626";
	Barra_2 = "X163626";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00670";
	TIPO = "Não Definida";
	Circuito_1 = "CIU-01W4";
	Circuito_2 = "AGS-01Z2";
	Barra_1 = "X452741";
	Barra_2 = "X452741";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00667";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F1";
	Circuito_2 = "AGS-01Z1";
	Barra_1 = "X124954";
	Barra_2 = "X124954";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00527";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F1";
	Circuito_2 = "AGS-01Z4";
	Barra_1 = "X197889";
	Barra_2 = "X197889";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00167";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F1";
	Circuito_2 = "CAJ-01P1";
	Barra_1 = "DX800601";
	Barra_2 = "DX800601";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00300";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F2";
	Circuito_2 = "CJD-01F8";
	Barra_1 = "104510617-1";
	Barra_2 = "104510617-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49769";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F2";
	Circuito_2 = "LBT-01S4";
	Barra_1 = "X128469";
	Barra_2 = "X128469";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00701";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F3";
	Circuito_2 = "CJD-01F4";
	Barra_1 = "X126262";
	Barra_2 = "X126262";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00702";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F3";
	Circuito_2 = "CJD-01F5";
	Barra_1 = "X153019";
	Barra_2 = "X153019";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F22684";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F3";
	Circuito_2 = "CJD-01F5";
	Barra_1 = "X153646";
	Barra_2 = "X153646";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53449";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F3";
	Circuito_2 = "CJD-01F6";
	Barra_1 = "12697333-1";
	Barra_2 = "12697333-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00415";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F4";
	Circuito_2 = "CJD-01F2";
	Barra_1 = "X126431";
	Barra_2 = "X126431";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00687";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F4";
	Circuito_2 = "CJD-01F2";
	Barra_1 = "X124136";
	Barra_2 = "X124136";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00528";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F4";
	Circuito_2 = "IPG-01C5";
	Barra_1 = "X126977";
	Barra_2 = "X126977";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49747";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F5";
	Circuito_2 = "CJD-01F4";
	Barra_1 = "X128987";
	Barra_2 = "X128987";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00610";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F5";
	Circuito_2 = "CJD-01F6";
	Barra_1 = "X415891";
	Barra_2 = "X415891";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00240";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F7";
	Circuito_2 = "CAJ-01P4";
	Barra_1 = "104739781-1";
	Barra_2 = "DX800602";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00236";
	TIPO = "Não Definida";
	Circuito_1 = "CJD-01F8";
	Circuito_2 = "CAJ-01P3";
	Barra_1 = "X147066";
	Barra_2 = "X147066";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00628";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X1";
	Circuito_2 = "CMU-01Y7";
	Barra_1 = "X403696";
	Barra_2 = "X403696";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00558";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X2";
	Circuito_2 = "CMT-01X3";
	Barra_1 = "9736916-1";
	Barra_2 = "9736916-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50447";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X2";
	Circuito_2 = "CMT-01X7";
	Barra_1 = "X175648";
	Barra_2 = "X175648";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49987";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X3";
	Circuito_2 = "CMT-01X5";
	Barra_1 = "X800901";
	Barra_2 = "X800901";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00631";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X3";
	Circuito_2 = "CMT-01X5";
	Barra_1 = "X184646";
	Barra_2 = "X184646";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00553";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X4";
	Circuito_2 = "CMT-01X1";
	Barra_1 = "X175529";
	Barra_2 = "X175529";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00467";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X4";
	Circuito_2 = "CMT-01X6";
	Barra_1 = "9656250-1";
	Barra_2 = "9656250-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56263";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X4";
	Circuito_2 = "CMT-01X6";
	Barra_1 = "X613902";
	Barra_2 = "X613902";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00756";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X4";
	Circuito_2 = "CMT-01X2";
	Barra_1 = "X468851";
	Barra_2 = "10035605-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56623";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X4";
	Circuito_2 = "CMU-01V1";
	Barra_1 = "X400672";
	Barra_2 = "X400672";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51592";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X5";
	Circuito_2 = "CMT-01X3";
	Barra_1 = "X703746";
	Barra_2 = "12469991-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00614";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X5";
	Circuito_2 = "CMT-01X2";
	Barra_1 = "HX406428";
	Barra_2 = "HX406428";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00464";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X5";
	Circuito_2 = "CMU-01Y8";
	Barra_1 = "12050681-1";
	Barra_2 = "12050681-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00676";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X6";
	Circuito_2 = "CMT-01X1";
	Barra_1 = "104464809-1";
	Barra_2 = "104464809-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51847";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X6";
	Circuito_2 = "CMT-01X1";
	Barra_1 = "104181098-1";
	Barra_2 = "X429760";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00612";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X6";
	Circuito_2 = "CMU-01Y3";
	Barra_1 = "X406555";
	Barra_2 = "X406555";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F55505";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X6";
	Circuito_2 = "CMU-01V1";
	Barra_1 = "X176455";
	Barra_2 = "X176455";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57200";
	TIPO = "Chave Fusível";
	Circuito_1 = "CMT-01X7";
	Circuito_2 = "CMT-01X7";
	Barra_1 = "X411601";
	Barra_2 = "104105826-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00615";
	TIPO = "Não Definida";
	Circuito_1 = "CMT-01X7";
	Circuito_2 = "CMT-01X1";
	Barra_1 = "X406567";
	Barra_2 = "X406567";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F55504";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CMU-01V1";
	Circuito_2 = "CMU-01V1";
	Barra_1 = "X177637";
	Barra_2 = "10540-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00457";
	TIPO = "Chave a óleo";
	Circuito_1 = "CMU-01V1";
	Circuito_2 = "CMU-01V1";
	Barra_1 = "12784624-1";
	Barra_2 = "12784624-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00475";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01V1";
	Circuito_2 = "CMU-01Y3";
	Barra_1 = "X176681";
	Barra_2 = "X176681";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00459";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01V1";
	Circuito_2 = "CMT-01X5";
	Barra_1 = "X176184";
	Barra_2 = "X176184";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00554";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y1";
	Circuito_2 = "CMU-01V1";
	Barra_1 = "X702421";
	Barra_2 = "X702421";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00458";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y1";
	Circuito_2 = "CMU-01Y8";
	Barra_1 = "X173422X";
	Barra_2 = "X173422X";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00618";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y1";
	Circuito_2 = "CMT-01X5";
	Barra_1 = "X181939";
	Barra_2 = "X181939";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00633";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y2";
	Circuito_2 = "CMU-01Y4";
	Barra_1 = "104316055-1";
	Barra_2 = "104316055-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00451";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y4";
	Circuito_2 = "CMU-01Y7";
	Barra_1 = "X804028";
	Barra_2 = "X804028";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52150";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y4";
	Circuito_2 = "CMT-01X6";
	Barra_1 = "X175415";
	Barra_2 = "X175415";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F40950B";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CMU-01Y6";
	Circuito_2 = "CMU-01Y6";
	Barra_1 = "12827551-1";
	Barra_2 = "12827551-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00692";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y6";
	Circuito_2 = "CMU-01Y1";
	Barra_1 = "X457014";
	Barra_2 = "X457014";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00552";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y7";
	Circuito_2 = "CMT-01X1";
	Barra_1 = "X193633";
	Barra_2 = "X193633";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00619";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y8";
	Circuito_2 = "CMU-01Y5";
	Barra_1 = "X407977";
	Barra_2 = "X407977";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54498";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y8";
	Circuito_2 = "CMU-01Y1";
	Barra_1 = "X567547";
	Barra_2 = "X567547";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F45791";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CMU-01Y9";
	Circuito_2 = "CMU-01Y9";
	Barra_1 = "12898697-1";
	Barra_2 = "12898697-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00620";
	TIPO = "Não Definida";
	Circuito_1 = "CMU-01Y9";
	Circuito_2 = "CMU-01Y6";
	Barra_1 = "12330358-1";
	Barra_2 = "12330358-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57397";
	TIPO = "Chave Fusível";
	Circuito_1 = "CPC-09Z1";
	Circuito_2 = "CPC-09Z1";
	Barra_1 = "X618145";
	Barra_2 = "12914257-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F46800B";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CPC-09Z1";
	Circuito_2 = "CPC-09Z1";
	Barra_1 = "12723417-1";
	Barra_2 = "12723417-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52134";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CPC-09Z2";
	Circuito_2 = "CPC-09Z2";
	Barra_1 = "12638240-1";
	Barra_2 = "12638240-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50081";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z2";
	Circuito_2 = "CPC-09Z1";
	Barra_1 = "X095213";
	Barra_2 = "X095213";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52786";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z3";
	Barra_1 = "12685042-1";
	Barra_2 = "12685042-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F55536";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z3";
	Barra_1 = "X211110";
	Barra_2 = "182574-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51142";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z2";
	Barra_1 = "12637415-1";
	Barra_2 = "12637415-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52821";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z7";
	Barra_1 = "X800806";
	Barra_2 = "X800806";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52920";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z2";
	Barra_1 = "X205286";
	Barra_2 = "X205286";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51141";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z3";
	Circuito_2 = "CPC-09Z2";
	Barra_1 = "X095001";
	Barra_2 = "X095001";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52205";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z4";
	Circuito_2 = "CPC-09Z5";
	Barra_1 = "X408325";
	Barra_2 = "X408325";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50760";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z5";
	Circuito_2 = "CPC-09Z4";
	Barra_1 = "X098751";
	Barra_2 = "X098751";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F40242";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z6";
	Circuito_2 = "CPC-09Z4";
	Barra_1 = "X095680";
	Barra_2 = "X095680";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49891";
	TIPO = "Seccionalizadora";
	Circuito_1 = "CPC-09Z7";
	Circuito_2 = "CPC-09Z7";
	Barra_1 = "12772235-1";
	Barra_2 = "12772235-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52820";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z7";
	Circuito_2 = "CPC-09Z1";
	Barra_1 = "X800370";
	Barra_2 = "X800370";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90921";
	TIPO = "Não Definida";
	Circuito_1 = "CPC-09Z8";
	Circuito_2 = "CPC-09Z6";
	Barra_1 = "12631928-1";
	Barra_2 = "12631928-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49116";
	TIPO = "Não Definida";
	Circuito_1 = "DJO-01V2";
	Circuito_2 = "DJO-01V1";
	Barra_1 = "X490016";
	Barra_2 = "X490016";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00446";
	TIPO = "Chave a óleo";
	Circuito_1 = "FDR-01J1";
	Circuito_2 = "FDR-01J1";
	Barra_1 = "104500594-1";
	Barra_2 = "104500594-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00173";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J1";
	Circuito_2 = "FDR-01Z2";
	Barra_1 = "X607992";
	Barra_2 = "X607992";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00543";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J1";
	Circuito_2 = "FDR-01Z3";
	Barra_1 = "9662826-1";
	Barra_2 = "9662826-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F38013";
	TIPO = "Chave Fusível";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "FDR-01J2";
	Barra_1 = "X111901";
	Barra_2 = "165743-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F02557";
	TIPO = "Seccionalizadora";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "FDR-01J2";
	Barra_1 = "12253239-1";
	Barra_2 = "12253239-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90711";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "FDR-01Z2";
	Barra_1 = "X116183";
	Barra_2 = "X116183";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F02281";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "FDR-01Z2";
	Barra_1 = "X112132";
	Barra_2 = "X112132";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00302";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "GRC-01L1";
	Barra_1 = "X099643";
	Barra_2 = "X099643";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53204";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J2";
	Circuito_2 = "GRC-01L1";
	Barra_1 = "X116123";
	Barra_2 = "X116123";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04040";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J3";
	Circuito_2 = "FDR-01J8";
	Barra_1 = "X112162";
	Barra_2 = "X112162";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "X12612";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J3";
	Circuito_2 = "FDR-01J5";
	Barra_1 = "172568-1";
	Barra_2 = "X112718";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51307";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J3";
	Circuito_2 = "FDR-01J8";
	Barra_1 = "X516949";
	Barra_2 = "X516949";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52175";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J3";
	Circuito_2 = "FDR-01J8";
	Barra_1 = "X561845";
	Barra_2 = "X561845";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00025";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J4";
	Circuito_2 = "FDR-01Z3";
	Barra_1 = "X063048";
	Barra_2 = "X063048";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90029";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J4";
	Circuito_2 = "FDR-01J1";
	Barra_1 = "172093-1";
	Barra_2 = "X111783";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00180";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J4";
	Circuito_2 = "FDR-01Z2";
	Barra_1 = "X114536";
	Barra_2 = "X114536";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00381";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J4";
	Circuito_2 = "CDL-01P7";
	Barra_1 = "X065777";
	Barra_2 = "X065777";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00306";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J5";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X113054";
	Barra_2 = "X113054";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F12610";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J5";
	Circuito_2 = "FDR-01J3";
	Barra_1 = "X112718";
	Barra_2 = "172568-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57100";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J5";
	Circuito_2 = "FDR-01Z3";
	Barra_1 = "X114953";
	Barra_2 = "X114953";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00137";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J5";
	Circuito_2 = "FNV-01V3";
	Barra_1 = "X115511";
	Barra_2 = "X115511";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00070";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J6";
	Circuito_2 = "CEN-01M1";
	Barra_1 = "X114806";
	Barra_2 = "X114806";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00034";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J6";
	Circuito_2 = "FDR-01J3";
	Barra_1 = "9765213-1";
	Barra_2 = "9765213-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00117";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J6";
	Circuito_2 = "FDR-01Z1";
	Barra_1 = "X112995";
	Barra_2 = "X112995";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56406";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J6";
	Circuito_2 = "FDR-01Z4";
	Barra_1 = "X114885";
	Barra_2 = "X114885";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00384";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J7";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X112658";
	Barra_2 = "X112658";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00196";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J7";
	Circuito_2 = "GRC-01Y1";
	Barra_1 = "X115099";
	Barra_2 = "X115099";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00074";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J7";
	Circuito_2 = "GRC-01Y7";
	Barra_1 = "X115468";
	Barra_2 = "X115468";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00037";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J8";
	Circuito_2 = "FDR-01J5";
	Barra_1 = "171760-2";
	Barra_2 = "X114821";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00165";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01J8";
	Circuito_2 = "FNV-01V3";
	Barra_1 = "9662813-1";
	Barra_2 = "9662813-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00121";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z1";
	Circuito_2 = "CEN-01J4";
	Barra_1 = "X135914";
	Barra_2 = "X135914";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54944";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z1";
	Circuito_2 = "FDR-01Z4";
	Barra_1 = "X114282";
	Barra_2 = "X114282";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00212";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z2";
	Circuito_2 = "FDR-01J2";
	Barra_1 = "104725565-1";
	Barra_2 = "104725565-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00448";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z2";
	Circuito_2 = "AML-01X2";
	Barra_1 = "X056189";
	Barra_2 = "X056189";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53432";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z2";
	Circuito_2 = "FDR-01J4";
	Barra_1 = "X112016";
	Barra_2 = "X112016";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00519";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z3";
	Circuito_2 = "FNV-01F1";
	Barra_1 = "X101051";
	Barra_2 = "X101051";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00277";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z3";
	Circuito_2 = "FNV-01F1";
	Barra_1 = "X113648";
	Barra_2 = "X113648";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00218";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z3";
	Circuito_2 = "MTT-01L3";
	Barra_1 = "X101488";
	Barra_2 = "X101488";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00394";
	TIPO = "Não Definida";
	Circuito_1 = "FDR-01Z4";
	Circuito_2 = "CEN-01X4";
	Barra_1 = "X074318";
	Barra_2 = "X074318";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26841";
	TIPO = "Seccionalizadora";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "FNV-01F1";
	Barra_1 = "X801973";
	Barra_2 = "510356-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F29893";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "FDR-01Z3";
	Barra_1 = "104725843-1";
	Barra_2 = "104725843-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00282";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "FDR-01J3";
	Barra_1 = "X112486";
	Barra_2 = "X112486";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51624";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "FNV-01V4";
	Barra_1 = "12495862-1";
	Barra_2 = "X517108";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51668";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "FNV-01V4";
	Barra_1 = "X469193";
	Barra_2 = "X469193";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52307";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F1";
	Circuito_2 = "MTT-01L2";
	Barra_1 = "X108644";
	Barra_2 = "X108644";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00018";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F5";
	Circuito_2 = "FDR-01Z1";
	Barra_1 = "9663016-1";
	Barra_2 = "X116411";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00525";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F5";
	Circuito_2 = "FNV-01V3";
	Barra_1 = "X117065";
	Barra_2 = "X117065";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52957";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F5";
	Circuito_2 = "CEN-01J5";
	Barra_1 = "X111497";
	Barra_2 = "X111497";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F01453";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01F5";
	Circuito_2 = "MTT-01N6";
	Barra_1 = "104725851-1";
	Barra_2 = "104725851-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00434";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V1";
	Circuito_2 = "CEN-01M3";
	Barra_1 = "X111168";
	Barra_2 = "X111168";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51368";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V1";
	Circuito_2 = "MTT-01N6";
	Barra_1 = "X111363";
	Barra_2 = "X111363";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00758";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V1";
	Circuito_2 = "MTT-01N6";
	Barra_1 = "X111342";
	Barra_2 = "X111342";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00388";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V2";
	Circuito_2 = "CEN-01M2";
	Barra_1 = "X111727";
	Barra_2 = "X111727";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F01536";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V2";
	Circuito_2 = "CEN-01M3";
	Barra_1 = "104725852-1";
	Barra_2 = "104725852-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53015";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V3";
	Circuito_2 = "FNV-01V4";
	Barra_1 = "X102943";
	Barra_2 = "X102943";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00010";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V4";
	Circuito_2 = "FNV-01F5";
	Barra_1 = "X102578";
	Barra_2 = "X102578";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50075";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V4";
	Circuito_2 = "FDR-01J5";
	Barra_1 = "X103549";
	Barra_2 = "X103549";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00522";
	TIPO = "Não Definida";
	Circuito_1 = "FNV-01V4";
	Circuito_2 = "MTT-01L1";
	Barra_1 = "X101195";
	Barra_2 = "X101195";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34701";
	TIPO = "Seccionalizadora";
	Circuito_1 = "GBA-01F1";
	Circuito_2 = "GBA-01F1";
	Barra_1 = "9860601-1";
	Barra_2 = "9860601-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00472";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F2";
	Circuito_2 = "GBA-01F3";
	Barra_1 = "9655106-1";
	Barra_2 = "9655106-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F40125";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F2";
	Circuito_2 = "GBA-01F4";
	Barra_1 = "X280113";
	Barra_2 = "X280113";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00473";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F3";
	Circuito_2 = "GBA-01F1";
	Barra_1 = "X188020";
	Barra_2 = "X188020";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F41864";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F4";
	Circuito_2 = "GBA-01F2";
	Barra_1 = "X602554";
	Barra_2 = "X602554";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53458";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F5";
	Circuito_2 = "GBA-01F6";
	Barra_1 = "X602392";
	Barra_2 = "X602392";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00496";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F5";
	Circuito_2 = "GBA-01F4";
	Barra_1 = "12740324-1";
	Barra_2 = "12740324-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53900";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F6";
	Circuito_2 = "GBA-01F1";
	Barra_1 = "X188656";
	Barra_2 = "X188656";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00454";
	TIPO = "Não Definida";
	Circuito_1 = "GBA-01F6";
	Circuito_2 = "ARB-01V4";
	Barra_1 = "X190024";
	Barra_2 = "X190024";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49804";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L1";
	Circuito_2 = "GRC-01L6";
	Barra_1 = "150843-1";
	Barra_2 = "X067833";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56569";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L1";
	Circuito_2 = "FDR-01J7";
	Barra_1 = "X115606";
	Barra_2 = "X115606";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50663";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L2";
	Circuito_2 = "GRC-01L1";
	Barra_1 = "12748062-1";
	Barra_2 = "12748062-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00209";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L2";
	Circuito_2 = "GRC-01L1";
	Barra_1 = "X066579";
	Barra_2 = "X066579";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00079";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L3";
	Circuito_2 = "GRC-01Y7";
	Barra_1 = "150779-1";
	Barra_2 = "X067304";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00237";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L3";
	Circuito_2 = "GRC-01L2";
	Barra_1 = "X070000";
	Barra_2 = "X070000";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00061";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L3";
	Circuito_2 = "GRC-01L4";
	Barra_1 = "X069837";
	Barra_2 = "X069837";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00730";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L3";
	Circuito_2 = "GRC-01L4";
	Barra_1 = "X461366";
	Barra_2 = "X461366";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00099";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L6";
	Circuito_2 = "GRC-01Y2";
	Barra_1 = "X071065";
	Barra_2 = "X071065";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00307";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01L6";
	Circuito_2 = "GRC-01L4";
	Barra_1 = "X467100";
	Barra_2 = "X467100";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F15059";
	TIPO = "Chave Fusível";
	Circuito_1 = "GRC-01Y2";
	Circuito_2 = "GRC-01Y2";
	Barra_1 = "104725858-1";
	Barra_2 = "104725858-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00329";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y2";
	Circuito_2 = "GRC-01Y1";
	Barra_1 = "X071401";
	Barra_2 = "X071401";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00094";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y2";
	Circuito_2 = "GRC-01Y5";
	Barra_1 = "X068488";
	Barra_2 = "X068488";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00163";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y2";
	Circuito_2 = "GRC-01Y1";
	Barra_1 = "X071288";
	Barra_2 = "X071288";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50442";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y5";
	Circuito_2 = "GRC-01Y7";
	Barra_1 = "X499888";
	Barra_2 = "X499888";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F47988";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y5";
	Circuito_2 = "FDR-01Z4";
	Barra_1 = "12333961-1";
	Barra_2 = "12333961-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00427";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y5";
	Circuito_2 = "GRC-01Y1";
	Barra_1 = "X067423";
	Barra_2 = "X067423";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00049";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y6";
	Circuito_2 = "GRC-01Y2";
	Barra_1 = "9443107-1";
	Barra_2 = "9443107-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50129";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y6";
	Circuito_2 = "GRC-01L6";
	Barra_1 = "X496252";
	Barra_2 = "X496252";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00362";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y6";
	Circuito_2 = "GRC-01L4";
	Barra_1 = "X073555";
	Barra_2 = "X073555";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00393";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y7";
	Circuito_2 = "GRC-01Y6";
	Barra_1 = "9649915-1";
	Barra_2 = "9649915-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00304";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y7";
	Circuito_2 = "FDR-01J3";
	Barra_1 = "12090642-1";
	Barra_2 = "12090642-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00389";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y7";
	Circuito_2 = "CEN-01X4";
	Barra_1 = "X117551";
	Barra_2 = "X117551";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00107";
	TIPO = "Não Definida";
	Circuito_1 = "GRC-01Y7";
	Circuito_2 = "FDR-01J3";
	Barra_1 = "9214448-1";
	Barra_2 = "9214448-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00351";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F1";
	Circuito_2 = "SCR-01J3";
	Barra_1 = "X210059";
	Barra_2 = "X210059";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56834";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F1";
	Circuito_2 = "SCR-01J3";
	Barra_1 = "X168629";
	Barra_2 = "X168629";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24617";
	TIPO = "Seccionalizadora";
	Circuito_1 = "IIN-01F2";
	Circuito_2 = "IIN-01F2";
	Barra_1 = "X501783";
	Barra_2 = "662847-1_AUX2";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24599";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F2";
	Circuito_2 = "SCR-01J1";
	Barra_1 = "X501725";
	Barra_2 = "X501725";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00200";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F2";
	Circuito_2 = "SCR-01N6";
	Barra_1 = "X210087";
	Barra_2 = "X210087";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52298";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F3";
	Circuito_2 = "IIN-01F2";
	Barra_1 = "X035455";
	Barra_2 = "X035455";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56601";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F3";
	Circuito_2 = "LDF-01M7";
	Barra_1 = "X026296";
	Barra_2 = "X026296";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56555";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F4";
	Circuito_2 = "IIN-01F1";
	Barra_1 = "X459403";
	Barra_2 = "X459403";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00724";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F4";
	Circuito_2 = "IIN-01F5";
	Barra_1 = "X040941";
	Barra_2 = "X040941";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90555";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F4";
	Circuito_2 = "LDF-01L2";
	Barra_1 = "X048696";
	Barra_2 = "X048696";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00333";
	TIPO = "Não Definida";
	Circuito_1 = "IIN-01F5";
	Circuito_2 = "SCR-01J4";
	Barra_1 = "X040496";
	Barra_2 = "X040496";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50275";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J1";
	Circuito_2 = "CAB-01S3";
	Barra_1 = "X001965";
	Barra_2 = "X001965";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00751";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J1";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "X012011";
	Barra_2 = "X012011";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54333";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J1";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "X012250";
	Barra_2 = "X012250";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00752";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J1";
	Circuito_2 = "CAB-01C6";
	Barra_1 = "X002001";
	Barra_2 = "X002001";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90113";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J1";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X010599";
	Barra_2 = "X010599";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54327";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J2";
	Circuito_2 = "PUD-01P5";
	Barra_1 = "X012789";
	Barra_2 = "X012789";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34711";
	TIPO = "Seccionalizadora";
	Circuito_1 = "IMB-01J3";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "104555221-1";
	Barra_2 = "104555221-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F48100";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J3";
	Circuito_2 = "IMB-01J1";
	Barra_1 = "X407692";
	Barra_2 = "X407692";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51956";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J3";
	Circuito_2 = "IMB-01J1";
	Barra_1 = "X564182";
	Barra_2 = "X564182";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00120";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J3";
	Circuito_2 = "IMB-01J2";
	Barra_1 = "X010185";
	Barra_2 = "X010185";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51333";
	TIPO = "Não Definida";
	Circuito_1 = "IMB-01J3";
	Circuito_2 = "CAB-01C1";
	Barra_1 = "X008894";
	Barra_2 = "X008894";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00062";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01C3";
	Circuito_2 = "IPG-01C5";
	Barra_1 = "X123980";
	Barra_2 = "X123879";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00146";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01C3";
	Circuito_2 = "LPN-01M4";
	Barra_1 = "X123980";
	Barra_2 = "X123980";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00686";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01C3";
	Circuito_2 = "LPN-01M6";
	Barra_1 = "X123258";
	Barra_2 = "X123258";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00114";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01C4";
	Circuito_2 = "IPG-01C5";
	Barra_1 = "147096-1";
	Barra_2 = "X138816";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00135";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01C5";
	Circuito_2 = "IPG-01C2";
	Barra_1 = "9442911-1";
	Barra_2 = "9442911-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00577";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z1";
	Circuito_2 = "IPG-01Z2";
	Barra_1 = "X134927";
	Barra_2 = "X134927";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90489";
	TIPO = "Chave Fusível";
	Circuito_1 = "IPG-01Z2";
	Circuito_2 = "IPG-01Z2";
	Barra_1 = "X141436";
	Barra_2 = "141402-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56553";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z2";
	Circuito_2 = "IPG-01Z3";
	Barra_1 = "9501132-1";
	Barra_2 = "9501132-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00246";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z2";
	Circuito_2 = "IPG-01Z3";
	Barra_1 = "X132831";
	Barra_2 = "X132831";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00039";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z2";
	Circuito_2 = "IPG-01Z4";
	Barra_1 = "X134758";
	Barra_2 = "142532-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90009";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z2";
	Circuito_2 = "IPG-01Z3";
	Barra_1 = "X140840";
	Barra_2 = "X140840";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00456";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z3";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "X132871";
	Barra_2 = "X132871";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00581";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z3";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "X132247";
	Barra_2 = "X132247";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00305";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z3";
	Circuito_2 = "IPG-01Z4";
	Barra_1 = "X137484";
	Barra_2 = "X137484";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90056";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z3";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "X133803";
	Barra_2 = "X133803";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00606";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z4";
	Circuito_2 = "IPG-01Z5";
	Barra_1 = "9533245-1";
	Barra_2 = "9533245-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00311";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z5";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "9655088-1";
	Barra_2 = "9655088-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00080";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z5";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "X123898";
	Barra_2 = "X123898";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04916";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z5";
	Circuito_2 = "IPG-01Z4";
	Barra_1 = "12463842-1";
	Barra_2 = "X138580";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00316";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z5";
	Circuito_2 = "IPG-01Z4";
	Barra_1 = "X197892";
	Barra_2 = "X197892";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53886";
	TIPO = "Não Definida";
	Circuito_1 = "IPG-01Z5";
	Circuito_2 = "LPN-01M6";
	Barra_1 = "12731374-1";
	Barra_2 = "12731374-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F91111";
	TIPO = "Não Definida";
	Circuito_1 = "LBT-01S1";
	Circuito_2 = "LBT-01S2";
	Barra_1 = "145368-1";
	Barra_2 = "X140399";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F36553";
	TIPO = "Não Definida";
	Circuito_1 = "LBT-01S3";
	Circuito_2 = "LBT-01S4";
	Barra_1 = "X134614";
	Barra_2 = "X134614";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00071";
	TIPO = "Não Definida";
	Circuito_1 = "LBT-01S3";
	Circuito_2 = "CJD-01F5";
	Barra_1 = "X153928";
	Barra_2 = "X153928";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56251";
	TIPO = "Seccionalizadora";
	Circuito_1 = "LBT-01S4";
	Circuito_2 = "LBT-01S4";
	Barra_1 = "X140051";
	Barra_2 = "138350-2_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54393";
	TIPO = "Chave Fusível";
	Circuito_1 = "LBT-01S4";
	Circuito_2 = "LBT-01S4";
	Barra_1 = "X137289";
	Barra_2 = "139979-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F26167";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L1";
	Circuito_2 = "LDF-01L3";
	Barra_1 = "X021454";
	Barra_2 = "X021454";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "M00338";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L1";
	Circuito_2 = "LDF-01M8";
	Barra_1 = "X022209";
	Barra_2 = "X022209";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F48524";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L1";
	Circuito_2 = "LDF-01M5";
	Barra_1 = "P530941";
	Barra_2 = "P530941";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50744";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L2";
	Circuito_2 = "LDF-01M1";
	Barra_1 = "X043490";
	Barra_2 = "X043490";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51422";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L2";
	Circuito_2 = "IIN-01F4";
	Barra_1 = "X044807";
	Barra_2 = "X044807";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50740";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L2";
	Circuito_2 = "IIN-01F5";
	Barra_1 = "X043393";
	Barra_2 = "X043393";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00243";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L3";
	Circuito_2 = "LDF-01M3";
	Barra_1 = "273996-1";
	Barra_2 = "X019314";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00417";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01L3";
	Circuito_2 = "LDF-01M6";
	Barra_1 = "P530943";
	Barra_2 = "P530943";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00647";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M1";
	Circuito_2 = "LDF-01M7";
	Barra_1 = "X414650";
	Barra_2 = "X414650";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00299";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M1";
	Circuito_2 = "IIN-01F5";
	Barra_1 = "X043399";
	Barra_2 = "X043399";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00422";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M1";
	Circuito_2 = "LDF-01M3";
	Barra_1 = "104725637-1";
	Barra_2 = "104725637-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00149";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M1";
	Circuito_2 = "LDF-01M7";
	Barra_1 = "X040904";
	Barra_2 = "X040904";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34792";
	TIPO = "Seccionalizadora";
	Circuito_1 = "LDF-01M2";
	Circuito_2 = "LDF-01M2";
	Barra_1 = "10007991-1";
	Barra_2 = "10007991-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49099";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M2";
	Circuito_2 = "LDF-01M8";
	Barra_1 = "X021532";
	Barra_2 = "X021532";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49982";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M2";
	Circuito_2 = "LDF-01L1";
	Barra_1 = "X037646";
	Barra_2 = "X037646";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00354";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M2";
	Circuito_2 = "LDF-01M6";
	Barra_1 = "X038001";
	Barra_2 = "X038001";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00303";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M2";
	Circuito_2 = "LDF-01M7";
	Barra_1 = "X501536";
	Barra_2 = "X501536";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F46363";
	TIPO = "Seccionalizadora";
	Circuito_1 = "LDF-01M4";
	Circuito_2 = "LDF-01M4";
	Barra_1 = "X033086";
	Barra_2 = "64835-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50344";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M5";
	Circuito_2 = "LDF-01L3";
	Barra_1 = "12437683-1";
	Barra_2 = "12437683-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00755";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M5";
	Circuito_2 = "LDF-01M4";
	Barra_1 = "X481945";
	Barra_2 = "X481945";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53305";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M5";
	Circuito_2 = "LDF-01M4";
	Barra_1 = "67538-1";
	Barra_2 = "X030788";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00412";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M6";
	Circuito_2 = "LDF-01M2";
	Barra_1 = "X038388";
	Barra_2 = "X038388";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56529";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M6";
	Circuito_2 = "LDF-01M9";
	Barra_1 = "12843042-1";
	Barra_2 = "12843042-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00050";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M7";
	Circuito_2 = "IIN-01F3";
	Barra_1 = "X036129";
	Barra_2 = "X036129";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00283";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M7";
	Circuito_2 = "IIN-01F5";
	Barra_1 = "X030047";
	Barra_2 = "X030047";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00370";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M7";
	Circuito_2 = "LDF-01M8";
	Barra_1 = "X043712";
	Barra_2 = "X043712";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00201";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M7";
	Circuito_2 = "LDF-01M8";
	Barra_1 = "71186-1";
	Barra_2 = "X021339";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00539";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M8";
	Circuito_2 = "IIN-01F3";
	Barra_1 = "X026734";
	Barra_2 = "X026734";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00536";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M8";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "X026499";
	Barra_2 = "X026499";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F44012";
	TIPO = "Chave Fusível";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "LDF-01M9";
	Barra_1 = "X034401";
	Barra_2 = "65075-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00095";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "LDF-01M4";
	Barra_1 = "X035628";
	Barra_2 = "X035628";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00555";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "ARB-01V1";
	Barra_1 = "X052812";
	Barra_2 = "X052812";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49095";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "LDF-01L1";
	Barra_1 = "X037299";
	Barra_2 = "X037299";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00595";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "LDF-01M3";
	Barra_1 = "X501541";
	Barra_2 = "X501541";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00754";
	TIPO = "Não Definida";
	Circuito_1 = "LDF-01M9";
	Circuito_2 = "LDF-01M5";
	Barra_1 = "X037225";
	Barra_2 = "X037225";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00067";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M1";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "X119938";
	Barra_2 = "X119938";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00431";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M1";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "156452-1";
	Barra_2 = "X120504";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00097";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M2";
	Circuito_2 = "LPN-01M1";
	Barra_1 = "X119913";
	Barra_2 = "X119913";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F20666";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M3";
	Circuito_2 = "FNV-01V1";
	Barra_1 = "12655979-1";
	Barra_2 = "12655979-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00723";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M3";
	Circuito_2 = "FNV-01V1";
	Barra_1 = "X123469";
	Barra_2 = "X123469";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00385";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M4";
	Circuito_2 = "IPG-01C4";
	Barra_1 = "X121359";
	Barra_2 = "X121359";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00104";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M4";
	Circuito_2 = "IPG-01Z5";
	Barra_1 = "X140256";
	Barra_2 = "X140256";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F13032";
	TIPO = "Chave Fusível";
	Circuito_1 = "LPN-01M5";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "X119982";
	Barra_2 = "156691-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04848";
	TIPO = "Seccionalizadora";
	Circuito_1 = "LPN-01M5";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "X120712";
	Barra_2 = "156523-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00132";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M5";
	Circuito_2 = "LPN-01M2";
	Barra_1 = "9627425-1";
	Barra_2 = "9627425-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52271";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M5";
	Circuito_2 = "LPN-01M4";
	Barra_1 = "X568445";
	Barra_2 = "X568445";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00125";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M6";
	Circuito_2 = "LPN-01M5";
	Barra_1 = "X120532";
	Barra_2 = "X120532";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00056";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M6";
	Circuito_2 = "IPG-01C3";
	Barra_1 = "X611840";
	Barra_2 = "X611840";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53713";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M6";
	Circuito_2 = "IPG-01Z5";
	Barra_1 = "X606549";
	Barra_2 = "X606549";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00219";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M7";
	Circuito_2 = "LPN-01M3";
	Barra_1 = "X119610";
	Barra_2 = "X119610";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00369";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M7";
	Circuito_2 = "LPN-01M4";
	Barra_1 = "X120749";
	Barra_2 = "X120749";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00127";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M8";
	Circuito_2 = "LPN-01M4";
	Barra_1 = "104295022-1";
	Barra_2 = "104295022-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00077";
	TIPO = "Não Definida";
	Circuito_1 = "LPN-01M8";
	Circuito_2 = "LPN-01M4";
	Barra_1 = "X121873";
	Barra_2 = "X121873";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00551";
	TIPO = "Não Definida";
	Circuito_1 = "MTR-01X1";
	Circuito_2 = "MTR-01X3";
	Barra_1 = "X092491";
	Barra_2 = "X092491";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52156";
	TIPO = "Seccionalizadora";
	Circuito_1 = "MTR-01X3";
	Circuito_2 = "MTR-01X3";
	Barra_1 = "12634769-1";
	Barra_2 = "12634769-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52199";
	TIPO = "Chave a óleo";
	Circuito_1 = "MTR-01X3";
	Circuito_2 = "MTR-01X3";
	Barra_1 = "X207652";
	Barra_2 = "178355-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00649";
	TIPO = "Não Definida";
	Circuito_1 = "MTR-01X5";
	Circuito_2 = "MTR-01X6";
	Barra_1 = "104187326-1";
	Barra_2 = "104187326-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00594";
	TIPO = "Não Definida";
	Circuito_1 = "MTR-01X6";
	Circuito_2 = "MTR-01X7";
	Barra_1 = "X096769";
	Barra_2 = "X096769";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00365";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01L1";
	Circuito_2 = "FNV-01V4";
	Barra_1 = "X101114";
	Barra_2 = "X101114";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00364";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01L1";
	Circuito_2 = "FNV-01F1";
	Barra_1 = "X101123";
	Barra_2 = "X101123";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00029";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01L3";
	Circuito_2 = "CDL-01P4";
	Barra_1 = "X102020";
	Barra_2 = "X102020";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00016";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01L4";
	Circuito_2 = "MTT-01L3";
	Barra_1 = "X101832";
	Barra_2 = "X101832";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00402";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01L6";
	Circuito_2 = "RET-01Y4";
	Barra_1 = "X107024";
	Barra_2 = "X107024";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54453";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N1";
	Circuito_2 = "MTT-01N4";
	Barra_1 = "X110182";
	Barra_2 = "X110182";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00759";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N1";
	Circuito_2 = "MTT-01N5";
	Barra_1 = "10212269-1";
	Barra_2 = "10212269-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00279";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N1";
	Circuito_2 = "MTT-01N3";
	Barra_1 = "X108991";
	Barra_2 = "X108991";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53841";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N2";
	Circuito_2 = "MTT-01N3";
	Barra_1 = "X108659";
	Barra_2 = "X108659";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00009";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N2";
	Circuito_2 = "LPN-01M3";
	Barra_1 = "X108306";
	Barra_2 = "X108306";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00766";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N2";
	Circuito_2 = "RET-01Y2";
	Barra_1 = "X107189";
	Barra_2 = "12060668-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00238";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N3";
	Circuito_2 = "MTT-01N2";
	Barra_1 = "X109008";
	Barra_2 = "X109008";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00765";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N4";
	Circuito_2 = "MTT-01N6";
	Barra_1 = "X102685";
	Barra_2 = "X102685";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F47958";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N4";
	Circuito_2 = "LPN-01M3";
	Barra_1 = "X487860";
	Barra_2 = "X487860";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F23724";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N4";
	Circuito_2 = "MTT-01N3";
	Barra_1 = "X432313";
	Barra_2 = "X432313";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00392";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N5";
	Circuito_2 = "CEN-01M3";
	Barra_1 = "104725636-1";
	Barra_2 = "104725636-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F31833";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N5";
	Circuito_2 = "CEN-01M3";
	Barra_1 = "9654993-1";
	Barra_2 = "9654993-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00001";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N5";
	Circuito_2 = "LPN-01M1";
	Barra_1 = "9526529-1";
	Barra_2 = "9526529-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00770";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N6";
	Circuito_2 = "MTT-01N5";
	Barra_1 = "X111307";
	Barra_2 = "X111307";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52932";
	TIPO = "Não Definida";
	Circuito_1 = "MTT-01N6";
	Circuito_2 = "FNV-01V1";
	Barra_1 = "X110598";
	Barra_2 = "X110598";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F27369";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PIT-01M1";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "8655641-1";
	Barra_2 = "8655641-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00408";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M1";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X022727";
	Barra_2 = "X022723";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00377";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M1";
	Circuito_2 = "AML-01S2";
	Barra_1 = "X501262";
	Barra_2 = "X501262";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "300604";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X023088";
	Barra_2 = "X023088";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00123";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "X023340";
	Barra_2 = "X023340";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F21963";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "104187396-1";
	Barra_2 = "104187396-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00541";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "X500219";
	Barra_2 = "X500219";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00181";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01M1";
	Barra_1 = "X501216";
	Barra_2 = "X501216";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00509";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X025056";
	Barra_2 = "X025056";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F12178";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X025196";
	Barra_2 = "X025196";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00517";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01Y5";
	Barra_1 = "X488983";
	Barra_2 = "X488983";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00376";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01M2";
	Circuito_2 = "PIT-01Y5";
	Barra_1 = "117254-1";
	Barra_2 = "X501217";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90059";
	TIPO = "Chave Fusível";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X010592";
	Barra_2 = "109663-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57252";
	TIPO = "Chave Fusível";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X617717";
	Barra_2 = "12900377-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56456";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "CDL-01W7";
	Barra_1 = "X001235";
	Barra_2 = "X001235";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90108";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "IMB-01J2";
	Barra_1 = "X023063";
	Barra_2 = "X023063";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00691";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "X010788";
	Barra_2 = "X010788";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00438";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y1";
	Circuito_2 = "PIT-01Y8";
	Barra_1 = "12597918-1";
	Barra_2 = "12597918-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F36831";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PIT-01Y2";
	Circuito_2 = "PIT-01Y2";
	Barra_1 = "12122350-1";
	Barra_2 = "12122350-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00540";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y2";
	Circuito_2 = "PIT-01Y3";
	Barra_1 = "DX500276";
	Barra_2 = "DX500276";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00531";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y2";
	Circuito_2 = "PIT-01Y3";
	Barra_1 = "X028071";
	Barra_2 = "X028071";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00444";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y2";
	Circuito_2 = "CDL-01W5";
	Barra_1 = "X032079";
	Barra_2 = "X032079";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F10176";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y3";
	Circuito_2 = "CDL-01P5";
	Barra_1 = "104725856-1";
	Barra_2 = "104725856-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00134";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y3";
	Circuito_2 = "CDL-01P5";
	Barra_1 = "X029174";
	Barra_2 = "X029174";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00102";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y3";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X028164";
	Barra_2 = "X028164";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00332";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y4";
	Circuito_2 = "PIT-01Y5";
	Barra_1 = "X025408";
	Barra_2 = "X025408";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00685";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X022654";
	Barra_2 = "X022654";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00089";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "CDL-01W3";
	Barra_1 = "X032803";
	Barra_2 = "X032803";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F20668";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "104725652-1";
	Barra_2 = "104725652-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00183";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X032152";
	Barra_2 = "X032152";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00533";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "PIT-01Y4";
	Barra_1 = "X025538";
	Barra_2 = "X025538";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F36340";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y5";
	Circuito_2 = "AML-01S3";
	Barra_1 = "X457452";
	Barra_2 = "X457452";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24834";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y6";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X030925";
	Barra_2 = "X030925";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00105";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y6";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "898940-1";
	Barra_2 = "898940-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00036";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y6";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X023403";
	Barra_2 = "X023403";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00407";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y7";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X029840";
	Barra_2 = "X029840";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24166";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y7";
	Circuito_2 = "PIT-01Y1";
	Barra_1 = "X501490";
	Barra_2 = "X501490";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00416";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y7";
	Circuito_2 = "PIT-01Y6";
	Barra_1 = "X022876";
	Barra_2 = "X022876";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00154";
	TIPO = "Não Definida";
	Circuito_1 = "PIT-01Y8";
	Circuito_2 = "PIT-01Y7";
	Barra_1 = "X028449";
	Barra_2 = "X028449";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00588";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y1";
	Circuito_2 = "PPE-01Y3";
	Barra_1 = "9214714-1";
	Barra_2 = "9214714-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00583";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y2";
	Circuito_2 = "PPE-01Y3";
	Barra_1 = "9214717-1";
	Barra_2 = "9214717-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00217";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y2";
	Circuito_2 = "PPE-01Y4";
	Barra_1 = "X076061";
	Barra_2 = "X076061";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00184";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y2";
	Circuito_2 = "PRP-01J2";
	Barra_1 = "X087475";
	Barra_2 = "X087475";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56732";
	TIPO = "Chave a óleo";
	Circuito_1 = "PPE-01Y3";
	Circuito_2 = "PPE-01Y3";
	Barra_1 = "X070570";
	Barra_2 = "38616-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56730";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y3";
	Circuito_2 = "PPE-01Y2";
	Barra_1 = "X612207";
	Barra_2 = "X612207";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57173";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y3";
	Circuito_2 = "PPE-01Y6";
	Barra_1 = "12865678-1";
	Barra_2 = "X615974";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00593";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y3";
	Circuito_2 = "PRP-01J1";
	Barra_1 = "X087498";
	Barra_2 = "X087498";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90371";
	TIPO = "Chave Fusível";
	Circuito_1 = "PPE-01Y4";
	Circuito_2 = "PPE-01Y4";
	Barra_1 = "X076517";
	Barra_2 = "34830-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57174";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y4";
	Circuito_2 = "PPE-01Y5";
	Barra_1 = "12865677-1";
	Barra_2 = "X615973";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00578";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y4";
	Circuito_2 = "PRP-01J2";
	Barra_1 = "9208111-1";
	Barra_2 = "9208111-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F38500";
	TIPO = "Não Definida";
	Circuito_1 = "PPE-01Y4";
	Circuito_2 = "PRP-01J4";
	Barra_1 = "X485455";
	Barra_2 = "X485455";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00576";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J2";
	Circuito_2 = "PRP-01J4";
	Barra_1 = "DX800849";
	Barra_2 = "DX800849";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00569";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J3";
	Circuito_2 = "CAJ-01P1";
	Barra_1 = "X156999";
	Barra_2 = "X156999";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00358";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J3";
	Circuito_2 = "PRP-01J1";
	Barra_1 = "9214377-1";
	Barra_2 = "9214377-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00188";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J4";
	Circuito_2 = "PRP-01J1";
	Barra_1 = "X085266";
	Barra_2 = "X085266";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00567";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J4";
	Circuito_2 = "PRP-01J3";
	Barra_1 = "9214731-1";
	Barra_2 = "9214731-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00287";
	TIPO = "Não Definida";
	Circuito_1 = "PRP-01J4";
	Circuito_2 = "PPE-01Y4";
	Barra_1 = "X081459";
	Barra_2 = "X081459";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F36820";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PTM-01W1";
	Circuito_2 = "PTM-01W1";
	Barra_1 = "12026645-1";
	Barra_2 = "12026645-1_AUX2";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34667";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PTM-01W1";
	Circuito_2 = "PTM-01W1";
	Barra_1 = "9859638-1";
	Barra_2 = "9859638-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51414";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W1";
	Circuito_2 = "PTM-01W4";
	Barra_1 = "54466-1";
	Barra_2 = "X024120";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50170";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W2";
	Circuito_2 = "PUD-01L3";
	Barra_1 = "12484481-1";
	Barra_2 = "12484481-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52487";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W3";
	Circuito_2 = "PUD-01L3";
	Barra_1 = "X009101";
	Barra_2 = "X009101";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00639";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W3";
	Circuito_2 = "SCR-01N8";
	Barra_1 = "X011187";
	Barra_2 = "X011187";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F53802";
	TIPO = "Chave a óleo";
	Circuito_1 = "PTM-01W4";
	Circuito_2 = "PTM-01W4";
	Barra_1 = "X471441";
	Barra_2 = "10198215-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56158";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W4";
	Circuito_2 = "PTM-01W5";
	Barra_1 = "X469255";
	Barra_2 = "X469255";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00605";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W5";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "X065013";
	Barra_2 = "X065013";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49857";
	TIPO = "Não Definida";
	Circuito_1 = "PTM-01W5";
	Circuito_2 = "SCR-01N8";
	Barra_1 = "X056632";
	Barra_2 = "X056632";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00178";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L1";
	Circuito_2 = "PUD-01P1";
	Barra_1 = "X015407";
	Barra_2 = "X015407";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00395";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L1";
	Circuito_2 = "PUD-01L2";
	Barra_1 = "9751891-1";
	Barra_2 = "9751891-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00654";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L1";
	Circuito_2 = "PUD-01L2";
	Barra_1 = "12445216-1";
	Barra_2 = "12445216-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51999";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L2";
	Circuito_2 = "PUD-01P5";
	Barra_1 = "X568110";
	Barra_2 = "X568110";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51834";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PUD-01L3";
	Circuito_2 = "PUD-01L3";
	Barra_1 = "X567341";
	Barra_2 = "12531326-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00656";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L3";
	Circuito_2 = "PUD-01L2";
	Barra_1 = "X005094";
	Barra_2 = "X005094";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50055";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L3";
	Circuito_2 = "PUD-01L2";
	Barra_1 = "X499483";
	Barra_2 = "X499483";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00157";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L4";
	Circuito_2 = "PUD-01P5";
	Barra_1 = "X004459";
	Barra_2 = "X004459";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24722";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L4";
	Circuito_2 = "PUD-01P5";
	Barra_1 = "104071735-1";
	Barra_2 = "104071735-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F23894";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L4";
	Circuito_2 = "PTM-01W1";
	Barra_1 = "X036420";
	Barra_2 = "X036420";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00168";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L4";
	Circuito_2 = "PUD-01L3";
	Barra_1 = "X004205";
	Barra_2 = "X004205";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00655";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L4";
	Circuito_2 = "PTM-01W1";
	Barra_1 = "X027347";
	Barra_2 = "X027347";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00420";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L5";
	Circuito_2 = "CJD-01F7";
	Barra_1 = "X203544";
	Barra_2 = "X203544";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00227";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L5";
	Circuito_2 = "PUD-01L1";
	Barra_1 = "X015261";
	Barra_2 = "X015261";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00664";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L5";
	Circuito_2 = "PUD-01P1";
	Barra_1 = "X020694";
	Barra_2 = "X020694";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50226";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L6";
	Circuito_2 = "PUD-01L7";
	Barra_1 = "X015873";
	Barra_2 = "X015873";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00665";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01L6";
	Circuito_2 = "CAB-01C2";
	Barra_1 = "X432269";
	Barra_2 = "X432269";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00145";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P1";
	Circuito_2 = "CJD-01F6";
	Barra_1 = "X033500";
	Barra_2 = "X033500";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00661";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P1";
	Circuito_2 = "PUD-01P3";
	Barra_1 = "X020128";
	Barra_2 = "X020128";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50136";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P2";
	Circuito_2 = "PUD-01L7";
	Barra_1 = "12870406-1";
	Barra_2 = "X616419";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00187";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P2";
	Circuito_2 = "CAB-01C2";
	Barra_1 = "12267103-1";
	Barra_2 = "12267103-2";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F32845";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P2";
	Circuito_2 = "CAB-01C4";
	Barra_1 = "X048212";
	Barra_2 = "X048212";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F34794";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P2";
	Circuito_2 = "PUD-01L6";
	Barra_1 = "104867761-2";
	Barra_2 = "104867761-2";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00666";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P2";
	Circuito_2 = "CAB-01C1";
	Barra_1 = "12325350-1";
	Barra_2 = "12325350-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50267";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P3";
	Circuito_2 = "PUD-01L7";
	Barra_1 = "12521612-1";
	Barra_2 = "12521612-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00440";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P3";
	Circuito_2 = "PUD-01L6";
	Barra_1 = "X019519";
	Barra_2 = "X019519";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52643";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PUD-01P4";
	Circuito_2 = "PUD-01P4";
	Barra_1 = "12622221-1";
	Barra_2 = "12622221-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52003";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P4";
	Circuito_2 = "PUD-01L4";
	Barra_1 = "12739767-1";
	Barra_2 = "12739767-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52645";
	TIPO = "Seccionalizadora";
	Circuito_1 = "PUD-01P5";
	Circuito_2 = "PUD-01P5";
	Barra_1 = "12622299-1";
	Barra_2 = "12622299-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52005";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P5";
	Circuito_2 = "PUD-01P4";
	Barra_1 = "95103-1";
	Barra_2 = "X004723";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00325";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P5";
	Circuito_2 = "IMB-01J2";
	Barra_1 = "X012703";
	Barra_2 = "X012703";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00322";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P5";
	Circuito_2 = "IMB-01J3";
	Barra_1 = "3493382-1";
	Barra_2 = "3493382-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24973";
	TIPO = "Não Definida";
	Circuito_1 = "PUD-01P5";
	Circuito_2 = "PUD-01L4";
	Barra_1 = "104071579-1";
	Barra_2 = "104071579-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57172";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F1";
	Circuito_2 = "CAB-01S2";
	Barra_1 = "X003056";
	Barra_2 = "X003056";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00680";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F1";
	Circuito_2 = "RET-01Y5";
	Barra_1 = "X104186";
	Barra_2 = "X104186";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F04049";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F2";
	Circuito_2 = "RET-01Y1";
	Barra_1 = "X134053";
	Barra_2 = "X134053";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49778";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F2";
	Circuito_2 = "LPN-01M8";
	Barra_1 = "X135197";
	Barra_2 = "X135197";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F57272";
	TIPO = "Chave Fusível";
	Circuito_1 = "RET-01F3";
	Circuito_2 = "RET-01F3";
	Barra_1 = "X137759";
	Barra_2 = "144226-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00066";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F3";
	Circuito_2 = "IPG-01C4";
	Barra_1 = "X131843";
	Barra_2 = "X131843";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00230";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F3";
	Circuito_2 = "RET-01F5";
	Barra_1 = "X135004";
	Barra_2 = "X135004";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49324";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F4";
	Circuito_2 = "RET-01F3";
	Barra_1 = "X108140";
	Barra_2 = "X108140";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00192";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F4";
	Circuito_2 = "CAB-01S2";
	Barra_1 = "X003015";
	Barra_2 = "X003015";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00060";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F5";
	Circuito_2 = "RET-01Y1";
	Barra_1 = "X107544";
	Barra_2 = "X107544";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00513";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01F5";
	Circuito_2 = "IPG-01C2";
	Barra_1 = "X131684";
	Barra_2 = "X131684";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00068";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y1";
	Circuito_2 = "LPN-01M7";
	Barra_1 = "X130320";
	Barra_2 = "X130320";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00161";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y3";
	Circuito_2 = "MTT-01N1";
	Barra_1 = "X106971";
	Barra_2 = "X106971";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00234";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y4";
	Circuito_2 = "CAB-01C7";
	Barra_1 = "X000988";
	Barra_2 = "X000988";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90915";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y4";
	Circuito_2 = "RET-01Y3";
	Barra_1 = "X495718";
	Barra_2 = "X495718";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00359";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y4";
	Circuito_2 = "MTT-01L5";
	Barra_1 = "X105518";
	Barra_2 = "X105518";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00115";
	TIPO = "Não Definida";
	Circuito_1 = "RET-01Y5";
	Circuito_2 = "MTT-01L5";
	Barra_1 = "X104374";
	Barra_2 = "X104374";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24640";
	TIPO = "Seccionalizadora";
	Circuito_1 = "SCR-01J1";
	Circuito_2 = "SCR-01J1";
	Barra_1 = "X700767";
	Barra_2 = "662718-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F24591";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J1";
	Circuito_2 = "IIN-01F2";
	Barra_1 = "X501707";
	Barra_2 = "X501707";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F29655";
	TIPO = "Seccionalizadora";
	Circuito_1 = "SCR-01J2";
	Circuito_2 = "SCR-01J2";
	Barra_1 = "9859612-1";
	Barra_2 = "9859612-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00734";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J2";
	Circuito_2 = "SCR-01J5";
	Barra_1 = "X041502";
	Barra_2 = "X041502";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00545";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J2";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "X041363";
	Barra_2 = "X041363";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F51298";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J2";
	Circuito_2 = "SCR-01N6";
	Barra_1 = "12526244-1";
	Barra_2 = "X705607";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00643";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J3";
	Circuito_2 = "IIN-01F4";
	Barra_1 = "X412079";
	Barra_2 = "X412079";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F54248";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J3";
	Circuito_2 = "CIT-01Y1";
	Barra_1 = "X569337";
	Barra_2 = "X569337";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50746";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J3";
	Circuito_2 = "IIN-01F1";
	Barra_1 = "X053772";
	Barra_2 = "X053772";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49759";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J3";
	Circuito_2 = "SCR-01N9";
	Barra_1 = "X045283";
	Barra_2 = "X045283";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00718";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J4";
	Circuito_2 = "IIN-01F4";
	Barra_1 = "X057174";
	Barra_2 = "X057174";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00309";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J5";
	Circuito_2 = "PTM-01W1";
	Barra_1 = "X031358";
	Barra_2 = "X031358";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00313";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J5";
	Circuito_2 = "SCR-01N3";
	Barra_1 = "X034641";
	Barra_2 = "X034641";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00590";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J6";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "X064653";
	Barra_2 = "X064653";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F50242";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01J6";
	Circuito_2 = "PTM-01W5";
	Barra_1 = "X469248";
	Barra_2 = "X469248";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F48432";
	TIPO = "Seccionalizadora";
	Circuito_1 = "SCR-01N2";
	Circuito_2 = "SCR-01N2";
	Barra_1 = "X035061";
	Barra_2 = "42484-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00735";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N2";
	Circuito_2 = "SCR-01J5";
	Barra_1 = "X039351";
	Barra_2 = "X039351";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00736";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N2";
	Circuito_2 = "SCR-01J5";
	Barra_1 = "X034771";
	Barra_2 = "X034771";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00397";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N2";
	Circuito_2 = "SCR-01N3";
	Barra_1 = "X200003";
	Barra_2 = "X200003";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F21306";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N2";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "X035090";
	Barra_2 = "X035090";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00314";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N3";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "X036804";
	Barra_2 = "X036804";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F29089";
	TIPO = "Seccionalizadora";
	Circuito_1 = "SCR-01N5";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "X501859";
	Barra_2 = "827144-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F09457";
	TIPO = "Chave Fusível";
	Circuito_1 = "SCR-01N5";
	Circuito_2 = "SCR-01N5";
	Barra_1 = "104227146-1";
	Barra_2 = "104227146-1_AUX1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00315";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N5";
	Circuito_2 = "SCR-01J4";
	Barra_1 = "X204087";
	Barra_2 = "X204087";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00579";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N6";
	Circuito_2 = "SCR-01J4";
	Barra_1 = "104811280-1";
	Barra_2 = "104811280-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F90678";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N6";
	Circuito_2 = "SCR-01J3";
	Barra_1 = "84513-1";
	Barra_2 = "X168589";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F91109";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N7";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "X056577";
	Barra_2 = "X056577";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00045";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N7";
	Circuito_2 = "SCR-01N9";
	Barra_1 = "X501962";
	Barra_2 = "X501962";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00636";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N7";
	Circuito_2 = "SCR-01N4";
	Barra_1 = "X056575";
	Barra_2 = "X056575";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F52739";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N7";
	Circuito_2 = "SCR-01N8";
	Barra_1 = "X700214";
	Barra_2 = "X700214";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00054";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N8";
	Circuito_2 = "SCR-01N7";
	Barra_1 = "X200160";
	Barra_2 = "X200160";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F00353";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N8";
	Circuito_2 = "CAJ-01P4";
	Barra_1 = "X015080";
	Barra_2 = "X015080";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F56272";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N8";
	Circuito_2 = "PUD-01L5";
	Barra_1 = "12822974-1";
	Barra_2 = "12822974-1";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);
	Chave = "F49757";
	TIPO = "Não Definida";
	Circuito_1 = "SCR-01N9";
	Circuito_2 = "SCR-01J6";
	Barra_1 = "X213207";
	Barra_2 = "X213207";
	if (!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2))
		rchRes->Lines->Add(Chave);

	edita->InsereLisEqpto(lisChavesNA);
	topologia->Reconfigura();

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEliminaFusivel::InsereChavesNA(AnsiString Chave, AnsiString TIPO,
	AnsiString Circuito_1, AnsiString Circuito_2, AnsiString Barra_1, AnsiString Barra_2)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoChave *tipoChave, *tipoFaca;

	VTRede *rede;
	TList *lisRede, *lisBarra, *lisTipoChave;
	VTChave *chaveNA;
	VTRede *rede1;
	VTBarra *barra1;
	VTRede *rede2;
	VTBarra *barra2;

	lisRede = redes->LisRede();
	// Busca rede1
	rede1 = NULL;
	for (int n = 0; n < lisRede->Count; n++)
	{
		rede1 = (VTRede*)lisRede->Items[n];
		if (rede1->Codigo.AnsiCompare(Circuito_1.Trim()) == 0)
			break;
		rede1 = NULL;
	}
	if (rede1 == NULL)
		return (false);

	// Busca rede2
	rede2 = NULL;
	for (int n = 0; n < lisRede->Count; n++)
	{
		rede2 = (VTRede*)lisRede->Items[n];
		if (rede2->Codigo.AnsiCompare(Circuito_2.Trim()) == 0)
			break;
		rede2 = NULL;
	}
	if (rede2 == NULL)
		return (false);

	if (rede1 == rede2)
		return (false);

	// busca barra1 da rede 1
	lisBarra = rede1->LisBarra();
	barra1 = NULL;
	for (int n = 0; n < lisBarra->Count; n++)
	{
		barra1 = (VTBarra*)lisBarra->Items[n];
		if (barra1->Codigo.AnsiCompare(Barra_1.Trim()) == 0)
			break;
		barra1 = NULL;
	}
	if (barra1 == NULL)
		return (false);

	// busca barra2 da rede 2
	lisBarra = rede2->LisBarra();
	barra2 = NULL;
	for (int n = 0; n < lisBarra->Count; n++)
	{
		barra2 = (VTBarra*)lisBarra->Items[n];
		if (barra2->Codigo.AnsiCompare(Barra_2.Trim()) == 0)
			break;
		barra2 = NULL;
	}
	if (barra2 == NULL)
		return (false);

	if (barra1 == barra2)
		return (false);

	lisTipoChave = tipos->LisTipoChave();
	tipoFaca = NULL;
	for (int n = 0; n < lisTipoChave->Count; n++)
	{
		tipoFaca = (VTTipoChave*)lisTipoChave->Items[n];
		if (tipoFaca->Codigo.AnsiCompare("Seccionadora") == 0)
			break;
		tipoFaca = NULL;
	}
	if (tipoFaca == NULL)
		return (false);

	tipoChave = NULL;
	for (int n = 0; n < lisTipoChave->Count; n++)
	{
		tipoChave = (VTTipoChave*)lisTipoChave->Items[n];
		if (tipoChave->Codigo.AnsiCompare(TIPO) == 0)
			break;
		tipoChave = NULL;
	}
	if (tipoChave == NULL)
		tipoChave = tipoFaca;

	chaveNA = DLL_NewObjChave(24);
	chaveNA->Codigo = Chave;
	chaveNA->DefineObjBarra(barra1, barra2);
	chaveNA->DefineObjRede(rede1);
	chaveNA->TipoChave = tipoChave;
	chaveNA->Status[sttNOVO] = true;
	chaveNA->Abre();
	lisChavesNA->Add(chaveNA);
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisAux = new TList();
	lisAux->Add(barra1);
	lisAux->Add(barra2);
	grafico->Moldura(lisAux);
	delete lisAux;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::EliminaChaves(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	grafico->Moldura();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	VTChave *chave;
	VTLigacao *ligacao;
	VTTrafo *trafo;
	TList *lisAux;
	TList *lisTrafo1, *lisTrafo2;
	bool iconta1, iconta2;
	VTBarra *barraTrafo, *barraRede;
	TList *lisBarraDeletar, *lisChaveDeletar;

	lisTrafo1 = new TList();
	lisTrafo2 = new TList();
	lisAux = new TList();
	lisBarraDeletar = new TList();
	lisChaveDeletar = new TList();

	for (int nc = 0; nc < lisChavesFunde->Count; nc++)
	{
		chave = (VTChave*)lisChavesFunde->Items[nc];
		lisTrafo1->Clear();
		lisTrafo2->Clear();
		// busca barra da chave ligada a trafo
		// na barra1
		lisAux->Clear();
		redes->LisLigacao(lisAux, chave->pbarra1);
		for (int nl = 0; nl < lisAux->Count; nl++)
		{
			ligacao = (VTLigacao*)lisAux->Items[nl];
			if (ligacao->Tipo() == eqptoTRAFO)
				lisTrafo1->Add(ligacao);
		}
		// na barra2
		lisAux->Clear();
		redes->LisLigacao(lisAux, chave->pbarra2);
		for (int nl = 0; nl < lisAux->Count; nl++)
		{
			ligacao = (VTLigacao*)lisAux->Items[nl];
			if (ligacao->Tipo() == eqptoTRAFO)
				lisTrafo2->Add(ligacao);
		}

		if ((lisTrafo1->Count > 0) && (lisTrafo2->Count > 0))
			continue;
		if ((lisTrafo1->Count > 1) || (lisTrafo2->Count > 1))
			continue;
		if ((lisTrafo1->Count == 0) && (lisTrafo2->Count == 0))
			continue;

		if (lisTrafo1->Count > 0)
		{
			trafo = (VTTrafo*)lisTrafo1->Items[0];
			barraTrafo = chave->pbarra1;
			barraRede = chave->pbarra2;
		}
		else
		{
			trafo = (VTTrafo*)lisTrafo2->Items[0];
			barraTrafo = chave->pbarra2;
			barraRede = chave->pbarra1;
		}
		edita->TrocaBarraLigacao(trafo, barraTrafo, barraRede);
		lisBarraDeletar->Add(barraTrafo);
		lisChaveDeletar->Add(chave);

	}

	// VTEqpto *eqpto;
	// for(int n = 0; n < lisChaveDeletar->Count; n++)
	// {
	// ligacao = (VTLigacao*)lisChaveDeletar->Items[n];
	// ligacao->pbarra1 = NULL;
	// ligacao->pbarra2 = NULL;
	// ligacao->pbarra3 = NULL;
	// ligacao->rede->RemoveLigacao(ligacao);
	// DLL_DeleteEqpto(ligacao); ligacao = NULL;
	// }
	// for(int n = 0; n < lisBarraDeletar->Count; n++)
	// {
	// eqpto = (VTEqpto*)lisBarraDeletar->Items[n];
	// redes->RemoveBarra((VTBarra*)eqpto);
	// DLL_DeleteEqpto(eqpto);
	// }

	// VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	// topologia->Reconfigura();

	// grafico->AreaRedeAltera();
	edita->RetiraLisEqpto(lisChaveDeletar);
	edita->RetiraLisEqpto(lisBarraDeletar);

	delete lisAux;
	delete lisTrafo1;
	delete lisTrafo2;
	delete lisBarraDeletar;
	delete lisChaveDeletar;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::EliminaChaves1(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	grafico->Moldura();
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	VTChave *chave;
	VTCarga *carga, *cargaClone;
	VTLigacao *ligacao;
	VTTrafo *trafo, *trafoClone;
	TList *lisAux;
	TList *lisTrafo1, *lisTrafo2;
	TList *lisEqBar;
	bool iconta1, iconta2;
	VTBarra *barraTrafo, *barraRede, *barraCarga, *barraPrimario;
	TList *lisEqptoDeletar, *lisEqptoInserir;

	lisTrafo1 = new TList();
	lisTrafo2 = new TList();
	lisAux = new TList();
	lisEqptoDeletar = new TList();
	lisEqptoInserir = new TList();

	for (int nc = 0; nc < lisChavesFunde->Count; nc++)
	{
		chave = (VTChave*)lisChavesFunde->Items[nc];
		lisTrafo1->Clear();
		lisTrafo2->Clear();
		// busca barra da chave ligada a trafo
		// na barra1
		lisAux->Clear();
		redes->LisLigacao(lisAux, chave->pbarra1);
		for (int nl = 0; nl < lisAux->Count; nl++)
		{
			ligacao = (VTLigacao*)lisAux->Items[nl];
			if (ligacao->Tipo() == eqptoTRAFO)
				lisTrafo1->Add(ligacao);
		}
		// na barra2
		lisAux->Clear();
		redes->LisLigacao(lisAux, chave->pbarra2);
		for (int nl = 0; nl < lisAux->Count; nl++)
		{
			ligacao = (VTLigacao*)lisAux->Items[nl];
			if (ligacao->Tipo() == eqptoTRAFO)
				lisTrafo2->Add(ligacao);
		}

		if ((lisTrafo1->Count > 0) && (lisTrafo2->Count > 0))
			continue;
		if ((lisTrafo1->Count > 1) || (lisTrafo2->Count > 1))
			continue;
		if ((lisTrafo1->Count == 0) && (lisTrafo2->Count == 0))
			continue;

		if (lisTrafo1->Count > 0)
		{
			trafo = (VTTrafo*)lisTrafo1->Items[0];
			barraCarga = trafo->BarraSecundario();
			lisEqBar = barraCarga->LisEqbar();
			if (lisEqBar->Count != 1)
			{
				continue;
			}

			barraTrafo = chave->pbarra1;
			barraRede = chave->pbarra2;
		}
		else
		{
			trafo = (VTTrafo*)lisTrafo2->Items[0];
			barraCarga = trafo->BarraSecundario();
			lisEqBar = barraCarga->LisEqbar();
			if (lisEqBar->Count != 1)
			{
				continue;
			}
			carga = (VTCarga*)lisEqBar->First();
			barraTrafo = chave->pbarra2;
			barraRede = chave->pbarra1;
		}

		cargaClone = carga->Clone();
		trafoClone = trafo->Clone();

		barraPrimario = trafoClone->BarraPrimario();
		if (trafoClone->pbarra1 == barraPrimario)
		{
			trafoClone->pbarra1 = barraRede;
		}
		else
		{
			if (trafoClone->pbarra2 == barraPrimario)
				trafoClone->pbarra2 = barraRede;
			else
			{
				if (trafoClone->pbarra3 == barraPrimario)
					trafoClone->pbarra3 = barraRede;
			}
		}

		// edita->TrocaBarraLigacao(trafo, barraTrafo, barraRede);
		lisEqptoDeletar->Add(carga);
		lisEqptoDeletar->Add(chave);
		lisEqptoDeletar->Add(trafo);

		lisEqptoInserir->Add(cargaClone);
		lisEqptoInserir->Add(trafoClone);

	}

	// VTEqpto *eqpto;
	// for(int n = 0; n < lisChaveDeletar->Count; n++)
	// {
	// ligacao = (VTLigacao*)lisChaveDeletar->Items[n];
	// ligacao->pbarra1 = NULL;
	// ligacao->pbarra2 = NULL;
	// ligacao->pbarra3 = NULL;
	// ligacao->rede->RemoveLigacao(ligacao);
	// DLL_DeleteEqpto(ligacao); ligacao = NULL;
	// }
	// for(int n = 0; n < lisBarraDeletar->Count; n++)
	// {
	// eqpto = (VTEqpto*)lisBarraDeletar->Items[n];
	// redes->RemoveBarra((VTBarra*)eqpto);
	// DLL_DeleteEqpto(eqpto);
	// }

	// VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	// topologia->Reconfigura();

	// grafico->AreaRedeAltera();
	edita->InsereLisEqpto(lisEqptoInserir);
	edita->RetiraLisEqpto(lisEqptoDeletar);

	delete lisAux;
	delete lisTrafo1;
	delete lisTrafo2;
	delete lisEqptoDeletar;
	delete lisEqptoInserir;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::IniciaLisChavesFunde(TList *lisChavesFunde)
{
	// Funcao que preenche lista de chaves para fusao de blocos
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTChave *chave;
	AnsiString codChave = "FU";

	lisChavesFunde->Clear();
	redes->LisEqpto(lisChavesFunde, eqptoCHAVE);
	for (int nc = lisChavesFunde->Count - 1; nc >= 0; nc--)
	{
		chave = (VTChave*)lisChavesFunde->Items[nc];
		if (chave->Codigo.AnsiCompare(codChave) == 0)
			continue;
		lisChavesFunde->Delete(nc);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEliminaFusivel::Button2Click(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->Moldura(lisChavesFunde);
}
// ---------------------------------------------------------------------------
/*
 Chave="F00694";TIPO="Não Definida";Circuito_1="AGS-01Z1";Circuito_2="AGS-01Z2";Barra_1="X158166";Barra_2="X158166";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00678";TIPO="Não Definida";Circuito_1="AGS-01Z1";Circuito_2="AGS-01Z5";Barra_1="X125863";Barra_2="X125863";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00681";TIPO="Não Definida";Circuito_1="AGS-01Z3";Circuito_2="AGS-01Z2";Barra_1="X158158";Barra_2="X158158";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00274";TIPO="Não Definida";Circuito_1="AGS-01Z4";Circuito_2="AGS-01Z3";Barra_1="X151587";Barra_2="X151587";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00693";TIPO="Não Definida";Circuito_1="AGS-01Z5";Circuito_2="AGS-01Z4";Barra_1="X457113";Barra_2="X457113";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00241";TIPO="Não Definida";Circuito_1="AGS-01Z5";Circuito_2="SCR-01N4";Barra_1="8715757-1";Barra_2="8715757-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00144";TIPO="Não Definida";Circuito_1="AML-01S1";Circuito_2="AML-01X1";Barra_1="X055184";Barra_2="X055184";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00357";TIPO="Não Definida";Circuito_1="AML-01S1";Circuito_2="AML-01X4";Barra_1="X062138";Barra_2="X062138";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00449";TIPO="Não Definida";Circuito_1="AML-01S1";Circuito_2="AML-01X2";Barra_1="X501463";Barra_2="X501463";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00336";TIPO="Não Definida";Circuito_1="AML-01S1";Circuito_2="FDR-01J4";Barra_1="X501464";Barra_2="X501464";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57147";TIPO="Não Definida";Circuito_1="AML-01S1";Circuito_2="FDR-01J4";Barra_1="12893532-1";Barra_2="12893532-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F28193";TIPO="Chave Fusível";Circuito_1="AML-01S2";Circuito_2="AML-01S2";Barra_1="12761406-1";Barra_2="12761406-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26253";TIPO="Seccionalizadora";Circuito_1="AML-01S2";Circuito_2="AML-01S2";Barra_1="X055710";Barra_2="118681-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26232";TIPO="Seccionalizadora";Circuito_1="AML-01S2";Circuito_2="AML-01S2";Barra_1="X055722";Barra_2="118493-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00048";TIPO="Não Definida";Circuito_1="AML-01S2";Circuito_2="PIT-01M2";Barra_1="104341197-1";Barra_2="104341197-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00518";TIPO="Não Definida";Circuito_1="AML-01S3";Circuito_2="PIT-01M2";Barra_1="9655097-1";Barra_2="9655097-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F14988";TIPO="Não Definida";Circuito_1="AML-01S3";Circuito_2="PIT-01M2";Barra_1="X023102";Barra_2="X023102";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53381";TIPO="Não Definida";Circuito_1="AML-01S3";Circuito_2="PIT-01M1";Barra_1="X023111";Barra_2="X023111";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00214";TIPO="Não Definida";Circuito_1="AML-01S4";Circuito_2="AML-01S3";Barra_1="X054585";Barra_2="X054585";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00715";TIPO="Não Definida";Circuito_1="AML-01S4";Circuito_2="AML-01X4";Barra_1="X460252";Barra_2="X460252";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26251";TIPO="Não Definida";Circuito_1="AML-01S5";Circuito_2="AML-01S2";Barra_1="X022809";Barra_2="X022809";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00247";TIPO="Não Definida";Circuito_1="AML-01S5";Circuito_2="AML-01S2";Barra_1="X022772";Barra_2="9655083-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00526";TIPO="Não Definida";Circuito_1="AML-01S5";Circuito_2="PIT-01Y5";Barra_1="X022722";Barra_2="X022722";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F09971";TIPO="Não Definida";Circuito_1="AML-01X1";Circuito_2="AML-01S2";Barra_1="X055801";Barra_2="X055801";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00445";TIPO="Não Definida";Circuito_1="AML-01X2";Circuito_2="AML-01X1";Barra_1="9622915-1";Barra_2="9622915-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F23389";TIPO="Não Definida";Circuito_1="AML-01X2";Circuito_2="AML-01X3";Barra_1="X059380";Barra_2="X059380";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00520";TIPO="Não Definida";Circuito_1="AML-01X2";Circuito_2="FDR-01J4";Barra_1="9627007-1";Barra_2="9627007-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00084";TIPO="Não Definida";Circuito_1="AML-01X2";Circuito_2="FDR-01Z3";Barra_1="X117838";Barra_2="X117838";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00521";TIPO="Não Definida";Circuito_1="AML-01X2";Circuito_2="FDR-01J1";Barra_1="X112372";Barra_2="X112372";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50140";TIPO="Não Definida";Circuito_1="AML-01X3";Circuito_2="AML-01X1";Barra_1="X499660";Barra_2="X499660";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00197";TIPO="Não Definida";Circuito_1="AML-01X4";Circuito_2="AML-01S5";Barra_1="X025232";Barra_2="X025232";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00532";TIPO="Não Definida";Circuito_1="AML-01X4";Circuito_2="CDL-01P7";Barra_1="X054980";Barra_2="X054980";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50369";TIPO="Não Definida";Circuito_1="AML-01X4";Circuito_2="FDR-01J4";Barra_1="X500433";Barra_2="X500433";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54801";TIPO="Não Definida";Circuito_1="ARB-01V1";Circuito_2="ARB-01V2";Barra_1="12732599-1";Barra_2="12732599-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53047";TIPO="Não Definida";Circuito_1="ARB-01V2";Circuito_2="LDF-01M4";Barra_1="X034017";Barra_2="X034017";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F14082";TIPO="Não Definida";Circuito_1="CAB-01C1";Circuito_2="CAB-01C4";Barra_1="104725643-1";Barra_2="104725643-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00319";TIPO="Não Definida";Circuito_1="CAB-01C1";Circuito_2="CAB-01C4";Barra_1="X607661";Barra_2="X607661";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00548";TIPO="Não Definida";Circuito_1="CAB-01C1";Circuito_2="PUD-01L6";Barra_1="12325352-1";Barra_2="12325352-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50452";TIPO="Não Definida";Circuito_1="CAB-01C2";Circuito_2="CAB-01C1";Barra_1="X008716";Barra_2="X008716";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00652";TIPO="Não Definida";Circuito_1="CAB-01C2";Circuito_2="IMB-01J3";Barra_1="X487618";Barra_2="X487618";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51996";TIPO="Não Definida";Circuito_1="CAB-01C2";Circuito_2="PUD-01P2";Barra_1="X008598";Barra_2="X008598";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00229";TIPO="Não Definida";Circuito_1="CAB-01C3";Circuito_2="CAB-01C2";Barra_1="X602378";Barra_2="X602378";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F23473";TIPO="Não Definida";Circuito_1="CAB-01C4";Circuito_2="CAB-01C6";Barra_1="X001769";Barra_2="X001769";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F09132";TIPO="Não Definida";Circuito_1="CAB-01C4";Circuito_2="CAB-01C6";Barra_1="104725541-1";Barra_2="104725541-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00410";TIPO="Não Definida";Circuito_1="CAB-01C4";Circuito_2="CAB-01C6";Barra_1="8363440-1";Barra_2="8363440-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00320";TIPO="Não Definida";Circuito_1="CAB-01C5";Circuito_2="CAB-01C7";Barra_1="9173181-1";Barra_2="9173181-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00193";TIPO="Não Definida";Circuito_1="CAB-01C5";Circuito_2="CAB-01S2";Barra_1="X002346";Barra_2="X002346";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57008";TIPO="Não Definida";Circuito_1="CAB-01C6";Circuito_2="CAB-01S3";Barra_1="X001873";Barra_2="X001873";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00255";TIPO="Não Definida";Circuito_1="CAB-01C6";Circuito_2="IMB-01J3";Barra_1="108636-1_AUX2";Barra_2="X002048";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00310";TIPO="Não Definida";Circuito_1="CAB-01C7";Circuito_2="CAB-01C6";Barra_1="X001825";Barra_2="X001825";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F55115";TIPO="Não Definida";Circuito_1="CAB-01C7";Circuito_2="MTT-01L6";Barra_1="X490922";Barra_2="X490922";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00396";TIPO="Não Definida";Circuito_1="CAB-01C8";Circuito_2="CAB-01C7";Barra_1="X000454";Barra_2="X000454";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00064";TIPO="Não Definida";Circuito_1="CAB-01S1";Circuito_2="CAB-01C8";Barra_1="X006640";Barra_2="X006640";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00289";TIPO="Não Definida";Circuito_1="CAB-01S1";Circuito_2="CJD-01F6";Barra_1="X154934";Barra_2="X154934";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00090";TIPO="Não Definida";Circuito_1="CAB-01S1";Circuito_2="RET-01F4";Barra_1="X014274";Barra_2="X014274";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00429";TIPO="Não Definida";Circuito_1="CAB-01S1";Circuito_2="RET-01F4";Barra_1="X155516";Barra_2="X155516";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00116";TIPO="Não Definida";Circuito_1="CAB-01S2";Circuito_2="RET-01F1";Barra_1="X003056";Barra_2="9309101-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00296";TIPO="Não Definida";Circuito_1="CAJ-01P1";Circuito_2="CAJ-01P2";Barra_1="X148164";Barra_2="X148164";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49991";TIPO="Não Definida";Circuito_1="CAJ-01P1";Circuito_2="LBT-01S2";Barra_1="X140702";Barra_2="X140702";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00148";TIPO="Não Definida";Circuito_1="CAJ-01P2";Circuito_2="CAJ-01P1";Barra_1="X147492";Barra_2="X147492";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50996";TIPO="Não Definida";Circuito_1="CAJ-01P2";Circuito_2="CAJ-01P3";Barra_1="X149623";Barra_2="X149623";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00659";TIPO="Não Definida";Circuito_1="CAJ-01P2";Circuito_2="AGS-01Z3";Barra_1="104393600-1";Barra_2="104393600-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00249";TIPO="Não Definida";Circuito_1="CAJ-01P2";Circuito_2="CJD-01F1";Barra_1="X124146";Barra_2="X124146";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F10798";TIPO="Chave Fusível";Circuito_1="CAJ-01P3";Circuito_2="CAJ-01P3";Barra_1="X149616";Barra_2="4614-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00669";TIPO="Não Definida";Circuito_1="CAJ-01P3";Circuito_2="AGS-01Z4";Barra_1="X150702";Barra_2="X150710";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00682";TIPO="Não Definida";Circuito_1="CAJ-01P3";Circuito_2="AGS-01Z3";Barra_1="X158868";Barra_2="X158868";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51035";TIPO="Seccionalizadora";Circuito_1="CAJ-01P4";Circuito_2="CAJ-01P4";Barra_1="X704813";Barra_2="12473868-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00668";TIPO="Não Definida";Circuito_1="CAJ-01P4";Circuito_2="CAJ-01P3";Barra_1="X124222";Barra_2="X150131";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52810";TIPO="Não Definida";Circuito_1="CAJ-01P4";Circuito_2="SCR-01N8";Barra_1="X149318";Barra_2="X149318";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50579";TIPO="Não Definida";Circuito_1="CAJ-01P4";Circuito_2="SCR-01N4";Barra_1="X700225";Barra_2="X700225";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00206";TIPO="Não Definida";Circuito_1="CDL-01P1";Circuito_2="CDL-01P5";Barra_1="X066178";Barra_2="X066178";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00355";TIPO="Não Definida";Circuito_1="CDL-01P1";Circuito_2="CDL-01P6";Barra_1="X453638";Barra_2="X453638";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00272";TIPO="Não Definida";Circuito_1="CDL-01P1";Circuito_2="CDL-01W1";Barra_1="X465343";Barra_2="X465343";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26992";TIPO="Não Definida";Circuito_1="CDL-01P1";Circuito_2="CDL-01W1";Barra_1="104067876-1";Barra_2="104067876-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26852";TIPO="Não Definida";Circuito_1="CDL-01P1";Circuito_2="CDL-01W4";Barra_1="X501371";Barra_2="X501371";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26850";TIPO="Não Definida";Circuito_1="CDL-01P2";Circuito_2="CDL-01W4";Barra_1="104725841-1";Barra_2="104725841-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00046";TIPO="Não Definida";Circuito_1="CDL-01P4";Circuito_2="CDL-01P3";Barra_1="7192519-1";Barra_2="7192519-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00073";TIPO="Não Definida";Circuito_1="CDL-01P4";Circuito_2="MTT-01L4";Barra_1="9465307-1";Barra_2="9465307-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00019";TIPO="Não Definida";Circuito_1="CDL-01P5";Circuito_2="CDL-01W2";Barra_1="9556626-1";Barra_2="9556626-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F32810";TIPO="Não Definida";Circuito_1="CDL-01P6";Circuito_2="MTT-01L5";Barra_1="X415198";Barra_2="X415198";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00098";TIPO="Não Definida";Circuito_1="CDL-01P6";Circuito_2="MTT-01L5";Barra_1="12536671-1";Barra_2="12536671-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00023";TIPO="Não Definida";Circuito_1="CDL-01P7";Circuito_2="AML-01S3";Barra_1="X053912";Barra_2="X053912";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00335";TIPO="Não Definida";Circuito_1="CDL-01P7";Circuito_2="CDL-01P3";Barra_1="104101839-1";Barra_2="104101839-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26544";TIPO="Não Definida";Circuito_1="CDL-01P7";Circuito_2="CDL-01W3";Barra_1="116979-1";Barra_2="X032820";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F36532";TIPO="Não Definida";Circuito_1="CDL-01P7";Circuito_2="FDR-01J4";Barra_1="X202457";Barra_2="X202457";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00198";TIPO="Não Definida";Circuito_1="CDL-01P8";Circuito_2="CDL-01P3";Barra_1="X200381";Barra_2="X200381";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50255";TIPO="Não Definida";Circuito_1="CDL-01P8";Circuito_2="CDL-01W3";Barra_1="X064937";Barra_2="X064937";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00383";TIPO="Não Definida";Circuito_1="CDL-01P8";Circuito_2="PIT-01Y3";Barra_1="9601422-1";Barra_2="9601422-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00155";TIPO="Não Definida";Circuito_1="CDL-01P8";Circuito_2="PIT-01Y4";Barra_1="X032842";Barra_2="X032842";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F20667";TIPO="Não Definida";Circuito_1="CDL-01P8";Circuito_2="PIT-01Y5";Barra_1="X032808";Barra_2="X032808";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00133";TIPO="Não Definida";Circuito_1="CDL-01W1";Circuito_2="CDL-01P1";Barra_1="X453640";Barra_2="X453640";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00360";TIPO="Não Definida";Circuito_1="CDL-01W2";Circuito_2="CDL-01P8";Barra_1="X065522";Barra_2="X065522";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00126";TIPO="Não Definida";Circuito_1="CDL-01W3";Circuito_2="CDL-01P3";Barra_1="7526648-1";Barra_2="7526648-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F27265";TIPO="Não Definida";Circuito_1="CDL-01W3";Circuito_2="CDL-01P3";Barra_1="X099913";Barra_2="X099913";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00675";TIPO="Não Definida";Circuito_1="CDL-01W5";Circuito_2="CDL-01W1";Barra_1="X453641";Barra_2="X453641";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26732";TIPO="Não Definida";Circuito_1="CDL-01W5";Circuito_2="PIT-01Y8";Barra_1="104068675-1";Barra_2="104068675-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00021";TIPO="Não Definida";Circuito_1="CDL-01W5";Circuito_2="PIT-01Y8";Barra_1="X032795";Barra_2="X032795";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51958";TIPO="Não Definida";Circuito_1="CDL-01W7";Circuito_2="CAB-01S3";Barra_1="X613658";Barra_2="X613658";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49337";TIPO="Não Definida";Circuito_1="CDL-01W7";Circuito_2="CDL-01W5";Barra_1="X001406";Barra_2="X001406";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00153";TIPO="Não Definida";Circuito_1="CDL-01W7";Circuito_2="CAB-01C7";Barra_1="X001432";Barra_2="X001432";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00374";TIPO="Não Definida";Circuito_1="CDL-01W7";Circuito_2="IMB-01J1";Barra_1="X010014";Barra_2="X010014";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00156";TIPO="Não Definida";Circuito_1="CDL-01W8";Circuito_2="CDL-01P6";Barra_1="X102859";Barra_2="X102859";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00535";TIPO="Não Definida";Circuito_1="CDL-01W8";Circuito_2="MTT-01L6";Barra_1="X102854";Barra_2="X102854";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00712";TIPO="Não Definida";Circuito_1="CDL-01W8";Circuito_2="MTT-01L4";Barra_1="X103136";Barra_2="X103136";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04226";TIPO="Não Definida";Circuito_1="CEN-01J4";Circuito_2="CEN-01P1";Barra_1="104159129-1";Barra_2="104159129-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F05727";TIPO="Não Definida";Circuito_1="CEN-01J4";Circuito_2="FDR-01Z4";Barra_1="X117788";Barra_2="X117788";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00124";TIPO="Não Definida";Circuito_1="CEN-01J5";Circuito_2="CEN-01M1";Barra_1="X701052";Barra_2="X701052";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00245";TIPO="Não Definida";Circuito_1="CEN-01J5";Circuito_2="FNV-01V2";Barra_1="X111614";Barra_2="X111614";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00390";TIPO="Não Definida";Circuito_1="CEN-01M1";Circuito_2="CEN-01X4";Barra_1="X701776";Barra_2="X701776";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F06349";TIPO="Seccionalizadora";Circuito_1="CEN-01M2";Circuito_2="CEN-01M2";Barra_1="X701642";Barra_2="164496-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00040";TIPO="Não Definida";Circuito_1="CEN-01M2";Circuito_2="CEN-01M3";Barra_1="X701439";Barra_2="X701439";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00078";TIPO="Não Definida";Circuito_1="CEN-01M2";Circuito_2="CEN-01X4";Barra_1="X702086";Barra_2="X702086";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F16405";TIPO="Não Definida";Circuito_1="CEN-01M2";Circuito_2="CEN-01J4";Barra_1="X073755";Barra_2="X073755";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00044";TIPO="Não Definida";Circuito_1="CEN-01M3";Circuito_2="FNV-01V2";Barra_1="X111709";Barra_2="X111709";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00352";TIPO="Não Definida";Circuito_1="CEN-01M3";Circuito_2="MTT-01N5";Barra_1="X111123";Barra_2="X111123";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50588";TIPO="Não Definida";Circuito_1="CEN-01P1";Circuito_2="CEN-01P2";Barra_1="12712853-1";Barra_2="12712853-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50584";TIPO="Não Definida";Circuito_1="CEN-01P1";Circuito_2="CEN-01P2";Barra_1="12712876-1";Barra_2="12712876-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50576";TIPO="Não Definida";Circuito_1="CEN-01P1";Circuito_2="CEN-01P2";Barra_1="12714546-1";Barra_2="12714546-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F48323";TIPO="Não Definida";Circuito_1="CEN-01P3";Circuito_2="LPN-01M5";Barra_1="12662812-1";Barra_2="12662812-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34797";TIPO="Seccionalizadora";Circuito_1="CEN-01X1";Circuito_2="CEN-01X1";Barra_1="12152658-1";Barra_2="12152658-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04405";TIPO="Não Definida";Circuito_1="CEN-01X1";Circuito_2="CEN-01J1";Barra_1="X702317";Barra_2="X702317";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F18687";TIPO="Não Definida";Circuito_1="CEN-01X1";Circuito_2="LPN-01M1";Barra_1="X119916";Barra_2="X119916";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50330";TIPO="Não Definida";Circuito_1="CEN-01X2";Circuito_2="CEN-01X1";Barra_1="12722380-1";Barra_2="12722380-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50336";TIPO="Não Definida";Circuito_1="CEN-01X2";Circuito_2="CEN-01X1";Barra_1="12740373-1";Barra_2="12740373-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50832";TIPO="Seccionalizadora";Circuito_1="CEN-01X4";Circuito_2="CEN-01X4";Barra_1="X703538";Barra_2="12439805-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="C50586";TIPO="Não Definida";Circuito_1="CEN-01X5";Circuito_2="CEN-01X2";Barra_1="12722370-1";Barra_2="12722370-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04395";TIPO="Não Definida";Circuito_1="CEN-01X5";Circuito_2="LPN-01M2";Barra_1="12662837-1";Barra_2="12662837-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00480";TIPO="Não Definida";Circuito_1="CID-01V1";Circuito_2="CID-01V2";Barra_1="X270283";Barra_2="X270283";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90050";TIPO="Seccionalizadora Tripolar";Circuito_1="CID-01V3";Circuito_2="CID-01V3";Barra_1="12537920-1";Barra_2="12537920-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00497";TIPO="Não Definida";Circuito_1="CID-01V3";Circuito_2="CID-01V1";Barra_1="X273081";Barra_2="X273081";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00498";TIPO="Não Definida";Circuito_1="CID-01V3";Circuito_2="MTR-01X1";Barra_1="X090304";Barra_2="X090304";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00591";TIPO="Não Definida";Circuito_1="CIT-01Y1";Circuito_2="CIU-01V3";Barra_1="9685442-2";Barra_2="9685442-2";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F47649";TIPO="Não Definida";Circuito_1="CIT-01Y1";Circuito_2="CIU-01V2";Barra_1="X163326";Barra_2="X163326";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00258";TIPO="Não Definida";Circuito_1="CIT-01Y1";Circuito_2="CIU-01V4";Barra_1="931620-1";Barra_2="931620-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00271";TIPO="Não Definida";Circuito_1="CIT-01Y1";Circuito_2="CIU-01W4";Barra_1="X466063";Barra_2="X466063";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00344";TIPO="Não Definida";Circuito_1="CIT-01Y2";Circuito_2="CIT-01Y3";Barra_1="X198346";Barra_2="X198346";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00719";TIPO="Não Definida";Circuito_1="CIT-01Y2";Circuito_2="CIT-01Y1";Barra_1="X146148";Barra_2="X146148";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00679";TIPO="Não Definida";Circuito_1="CIT-01Y2";Circuito_2="IIN-01F4";Barra_1="X168280";Barra_2="X168280";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00269";TIPO="Não Definida";Circuito_1="CIT-01Y3";Circuito_2="CIU-01V5";Barra_1="X488691";Barra_2="X488691";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00262";TIPO="Não Definida";Circuito_1="CIT-01Y3";Circuito_2="CIU-01V6";Barra_1="X146158";Barra_2="X146158";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51179";TIPO="Chave a óleo";Circuito_1="CIT-01Y4";Circuito_2="CIT-01Y4";Barra_1="X700363";Barra_2="12484566-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56439";TIPO="Não Definida";Circuito_1="CIT-01Y4";Circuito_2="CIT-01Y6";Barra_1="X146856";Barra_2="X146856";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00697";TIPO="Não Definida";Circuito_1="CIT-01Y4";Circuito_2="CIU-01V3";Barra_1="X160246";Barra_2="X160246";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00574";TIPO="Não Definida";Circuito_1="CIT-01Y4";Circuito_2="CIU-01V5";Barra_1="X143812";Barra_2="X143812";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56738";TIPO="Chave a óleo";Circuito_1="CIT-01Y6";Circuito_2="CIT-01Y6";Barra_1="X608480";Barra_2="12756458-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56349";TIPO="Não Definida";Circuito_1="CIT-01Y6";Circuito_2="CIT-01Y4";Barra_1="X608507";Barra_2="X608507";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00347";TIPO="Não Definida";Circuito_1="CIU-01V1";Circuito_2="CIU-01W1";Barra_1="X079133";Barra_2="X079133";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90904";TIPO="Não Definida";Circuito_1="CIU-01V2";Circuito_2="CIT-01Y2";Barra_1="12794458-1";Barra_2="12794458-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00324";TIPO="Não Definida";Circuito_1="CIU-01V2";Circuito_2="CIU-01W3";Barra_1="X162999";Barra_2="X162999";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90502";TIPO="Não Definida";Circuito_1="CIU-01V3";Circuito_2="CIT-01Y4";Barra_1="X700362";Barra_2="X700362";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00642";TIPO="Não Definida";Circuito_1="CIU-01V3";Circuito_2="CIU-01V1";Barra_1="X162409";Barra_2="X162409";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00350";TIPO="Não Definida";Circuito_1="CIU-01V3";Circuito_2="CIU-01W4";Barra_1="104916009-1";Barra_2="104916009-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00295";TIPO="Não Definida";Circuito_1="CIU-01V4";Circuito_2="CIU-01W1";Barra_1="D163529X";Barra_2="D163529X";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00281";TIPO="Não Definida";Circuito_1="CIU-01V5";Circuito_2="CIU-01V6";Barra_1="X400870";Barra_2="X198339";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00035";TIPO="Não Definida";Circuito_1="CIU-01V6";Circuito_2="CIT-01Y1";Barra_1="X466877";Barra_2="X466877";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00020";TIPO="Não Definida";Circuito_1="CIU-01V6";Circuito_2="CIU-01V2";Barra_1="12463806-1";Barra_2="12463806-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00349";TIPO="Não Definida";Circuito_1="CIU-01V6";Circuito_2="CIU-01V3";Barra_1="X198337";Barra_2="X198337";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F17645";TIPO="Não Definida";Circuito_1="CIU-01V7";Circuito_2="CIU-01V8";Barra_1="104725540-1";Barra_2="104725540-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00732";TIPO="Não Definida";Circuito_1="CIU-01V7";Circuito_2="CIU-01V4";Barra_1="X163485";Barra_2="X163485";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90911";TIPO="Não Definida";Circuito_1="CIU-01V7";Circuito_2="CIU-01V8";Barra_1="12769801-1";Barra_2="12769801-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00337";TIPO="Não Definida";Circuito_1="CIU-01V8";Circuito_2="CIU-01V7";Barra_1="X163465";Barra_2="X163465";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00093";TIPO="Não Definida";Circuito_1="CIU-01V8";Circuito_2="CIU-01V7";Barra_1="X163407";Barra_2="X163407";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00292";TIPO="Não Definida";Circuito_1="CIU-01V8";Circuito_2="PPE-01Y1";Barra_1="X163682";Barra_2="X163682";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00250";TIPO="Não Definida";Circuito_1="CIU-01W1";Circuito_2="CIU-01V4";Barra_1="X152952";Barra_2="X152952";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00749";TIPO="Não Definida";Circuito_1="CIU-01W2";Circuito_2="CIU-01W1";Barra_1="X468003";Barra_2="X468003";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00286";TIPO="Não Definida";Circuito_1="CIU-01W2";Circuito_2="PPE-01Y4";Barra_1="X078065";Barra_2="X078065";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00348";TIPO="Não Definida";Circuito_1="CIU-01W3";Circuito_2="CIU-01V1";Barra_1="X163626";Barra_2="X163626";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00670";TIPO="Não Definida";Circuito_1="CIU-01W4";Circuito_2="AGS-01Z2";Barra_1="X452741";Barra_2="X452741";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00667";TIPO="Não Definida";Circuito_1="CJD-01F1";Circuito_2="AGS-01Z1";Barra_1="X124954";Barra_2="X124954";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00527";TIPO="Não Definida";Circuito_1="CJD-01F1";Circuito_2="AGS-01Z4";Barra_1="X197889";Barra_2="X197889";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00167";TIPO="Não Definida";Circuito_1="CJD-01F1";Circuito_2="CAJ-01P1";Barra_1="DX800601";Barra_2="DX800601";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00300";TIPO="Não Definida";Circuito_1="CJD-01F2";Circuito_2="CJD-01F8";Barra_1="104510617-1";Barra_2="104510617-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49769";TIPO="Não Definida";Circuito_1="CJD-01F2";Circuito_2="LBT-01S4";Barra_1="X128469";Barra_2="X128469";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00701";TIPO="Não Definida";Circuito_1="CJD-01F3";Circuito_2="CJD-01F4";Barra_1="X126262";Barra_2="X126262";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00702";TIPO="Não Definida";Circuito_1="CJD-01F3";Circuito_2="CJD-01F5";Barra_1="X153019";Barra_2="X153019";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F22684";TIPO="Não Definida";Circuito_1="CJD-01F3";Circuito_2="CJD-01F5";Barra_1="X153646";Barra_2="X153646";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53449";TIPO="Não Definida";Circuito_1="CJD-01F3";Circuito_2="CJD-01F6";Barra_1="12697333-1";Barra_2="12697333-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00415";TIPO="Não Definida";Circuito_1="CJD-01F4";Circuito_2="CJD-01F2";Barra_1="X126431";Barra_2="X126431";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00687";TIPO="Não Definida";Circuito_1="CJD-01F4";Circuito_2="CJD-01F2";Barra_1="X124136";Barra_2="X124136";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00528";TIPO="Não Definida";Circuito_1="CJD-01F4";Circuito_2="IPG-01C5";Barra_1="X126977";Barra_2="X126977";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49747";TIPO="Não Definida";Circuito_1="CJD-01F5";Circuito_2="CJD-01F4";Barra_1="X128987";Barra_2="X128987";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00610";TIPO="Não Definida";Circuito_1="CJD-01F5";Circuito_2="CJD-01F6";Barra_1="X415891";Barra_2="X415891";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00240";TIPO="Não Definida";Circuito_1="CJD-01F7";Circuito_2="CAJ-01P4";Barra_1="104739781-1";Barra_2="DX800602";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00236";TIPO="Não Definida";Circuito_1="CJD-01F8";Circuito_2="CAJ-01P3";Barra_1="X147066";Barra_2="X147066";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00628";TIPO="Não Definida";Circuito_1="CMT-01X1";Circuito_2="CMU-01Y7";Barra_1="X403696";Barra_2="X403696";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00558";TIPO="Não Definida";Circuito_1="CMT-01X2";Circuito_2="CMT-01X3";Barra_1="9736916-1";Barra_2="9736916-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50447";TIPO="Não Definida";Circuito_1="CMT-01X2";Circuito_2="CMT-01X7";Barra_1="X175648";Barra_2="X175648";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49987";TIPO="Não Definida";Circuito_1="CMT-01X3";Circuito_2="CMT-01X5";Barra_1="X800901";Barra_2="X800901";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00631";TIPO="Não Definida";Circuito_1="CMT-01X3";Circuito_2="CMT-01X5";Barra_1="X184646";Barra_2="X184646";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00553";TIPO="Não Definida";Circuito_1="CMT-01X4";Circuito_2="CMT-01X1";Barra_1="X175529";Barra_2="X175529";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00467";TIPO="Não Definida";Circuito_1="CMT-01X4";Circuito_2="CMT-01X6";Barra_1="9656250-1";Barra_2="9656250-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56263";TIPO="Não Definida";Circuito_1="CMT-01X4";Circuito_2="CMT-01X6";Barra_1="X613902";Barra_2="X613902";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00756";TIPO="Não Definida";Circuito_1="CMT-01X4";Circuito_2="CMT-01X2";Barra_1="X468851";Barra_2="10035605-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56623";TIPO="Não Definida";Circuito_1="CMT-01X4";Circuito_2="CMU-01V1";Barra_1="X400672";Barra_2="X400672";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51592";TIPO="Não Definida";Circuito_1="CMT-01X5";Circuito_2="CMT-01X3";Barra_1="X703746";Barra_2="12469991-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00614";TIPO="Não Definida";Circuito_1="CMT-01X5";Circuito_2="CMT-01X2";Barra_1="HX406428";Barra_2="HX406428";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00464";TIPO="Não Definida";Circuito_1="CMT-01X5";Circuito_2="CMU-01Y8";Barra_1="12050681-1";Barra_2="12050681-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00676";TIPO="Não Definida";Circuito_1="CMT-01X6";Circuito_2="CMT-01X1";Barra_1="104464809-1";Barra_2="104464809-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51847";TIPO="Não Definida";Circuito_1="CMT-01X6";Circuito_2="CMT-01X1";Barra_1="104181098-1";Barra_2="X429760";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00612";TIPO="Não Definida";Circuito_1="CMT-01X6";Circuito_2="CMU-01Y3";Barra_1="X406555";Barra_2="X406555";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F55505";TIPO="Não Definida";Circuito_1="CMT-01X6";Circuito_2="CMU-01V1";Barra_1="X176455";Barra_2="X176455";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57200";TIPO="Chave Fusível";Circuito_1="CMT-01X7";Circuito_2="CMT-01X7";Barra_1="X411601";Barra_2="104105826-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00615";TIPO="Não Definida";Circuito_1="CMT-01X7";Circuito_2="CMT-01X1";Barra_1="X406567";Barra_2="X406567";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F55504";TIPO="Seccionalizadora";Circuito_1="CMU-01V1";Circuito_2="CMU-01V1";Barra_1="X177637";Barra_2="10540-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00457";TIPO="Chave a óleo";Circuito_1="CMU-01V1";Circuito_2="CMU-01V1";Barra_1="12784624-1";Barra_2="12784624-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00475";TIPO="Não Definida";Circuito_1="CMU-01V1";Circuito_2="CMU-01Y3";Barra_1="X176681";Barra_2="X176681";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00459";TIPO="Não Definida";Circuito_1="CMU-01V1";Circuito_2="CMT-01X5";Barra_1="X176184";Barra_2="X176184";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00554";TIPO="Não Definida";Circuito_1="CMU-01Y1";Circuito_2="CMU-01V1";Barra_1="X702421";Barra_2="X702421";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00458";TIPO="Não Definida";Circuito_1="CMU-01Y1";Circuito_2="CMU-01Y8";Barra_1="X173422X";Barra_2="X173422X";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00618";TIPO="Não Definida";Circuito_1="CMU-01Y1";Circuito_2="CMT-01X5";Barra_1="X181939";Barra_2="X181939";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00633";TIPO="Não Definida";Circuito_1="CMU-01Y2";Circuito_2="CMU-01Y4";Barra_1="104316055-1";Barra_2="104316055-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00451";TIPO="Não Definida";Circuito_1="CMU-01Y4";Circuito_2="CMU-01Y7";Barra_1="X804028";Barra_2="X804028";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52150";TIPO="Não Definida";Circuito_1="CMU-01Y4";Circuito_2="CMT-01X6";Barra_1="X175415";Barra_2="X175415";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F40950B";TIPO="Seccionalizadora";Circuito_1="CMU-01Y6";Circuito_2="CMU-01Y6";Barra_1="12827551-1";Barra_2="12827551-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00692";TIPO="Não Definida";Circuito_1="CMU-01Y6";Circuito_2="CMU-01Y1";Barra_1="X457014";Barra_2="X457014";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00552";TIPO="Não Definida";Circuito_1="CMU-01Y7";Circuito_2="CMT-01X1";Barra_1="X193633";Barra_2="X193633";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00619";TIPO="Não Definida";Circuito_1="CMU-01Y8";Circuito_2="CMU-01Y5";Barra_1="X407977";Barra_2="X407977";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54498";TIPO="Não Definida";Circuito_1="CMU-01Y8";Circuito_2="CMU-01Y1";Barra_1="X567547";Barra_2="X567547";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F45791";TIPO="Seccionalizadora";Circuito_1="CMU-01Y9";Circuito_2="CMU-01Y9";Barra_1="12898697-1";Barra_2="12898697-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00620";TIPO="Não Definida";Circuito_1="CMU-01Y9";Circuito_2="CMU-01Y6";Barra_1="12330358-1";Barra_2="12330358-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57397";TIPO="Chave Fusível";Circuito_1="CPC-09Z1";Circuito_2="CPC-09Z1";Barra_1="X618145";Barra_2="12914257-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F46800B";TIPO="Seccionalizadora";Circuito_1="CPC-09Z1";Circuito_2="CPC-09Z1";Barra_1="12723417-1";Barra_2="12723417-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52134";TIPO="Seccionalizadora";Circuito_1="CPC-09Z2";Circuito_2="CPC-09Z2";Barra_1="12638240-1";Barra_2="12638240-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50081";TIPO="Não Definida";Circuito_1="CPC-09Z2";Circuito_2="CPC-09Z1";Barra_1="X095213";Barra_2="X095213";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52786";TIPO="Seccionalizadora";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z3";Barra_1="12685042-1";Barra_2="12685042-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F55536";TIPO="Seccionalizadora";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z3";Barra_1="X211110";Barra_2="182574-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51142";TIPO="Não Definida";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z2";Barra_1="12637415-1";Barra_2="12637415-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52821";TIPO="Não Definida";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z7";Barra_1="X800806";Barra_2="X800806";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52920";TIPO="Não Definida";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z2";Barra_1="X205286";Barra_2="X205286";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51141";TIPO="Não Definida";Circuito_1="CPC-09Z3";Circuito_2="CPC-09Z2";Barra_1="X095001";Barra_2="X095001";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52205";TIPO="Não Definida";Circuito_1="CPC-09Z4";Circuito_2="CPC-09Z5";Barra_1="X408325";Barra_2="X408325";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50760";TIPO="Não Definida";Circuito_1="CPC-09Z5";Circuito_2="CPC-09Z4";Barra_1="X098751";Barra_2="X098751";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F40242";TIPO="Não Definida";Circuito_1="CPC-09Z6";Circuito_2="CPC-09Z4";Barra_1="X095680";Barra_2="X095680";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49891";TIPO="Seccionalizadora";Circuito_1="CPC-09Z7";Circuito_2="CPC-09Z7";Barra_1="12772235-1";Barra_2="12772235-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52820";TIPO="Não Definida";Circuito_1="CPC-09Z7";Circuito_2="CPC-09Z1";Barra_1="X800370";Barra_2="X800370";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90921";TIPO="Não Definida";Circuito_1="CPC-09Z8";Circuito_2="CPC-09Z6";Barra_1="12631928-1";Barra_2="12631928-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49116";TIPO="Não Definida";Circuito_1="DJO-01V2";Circuito_2="DJO-01V1";Barra_1="X490016";Barra_2="X490016";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00446";TIPO="Chave a óleo";Circuito_1="FDR-01J1";Circuito_2="FDR-01J1";Barra_1="104500594-1";Barra_2="104500594-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00173";TIPO="Não Definida";Circuito_1="FDR-01J1";Circuito_2="FDR-01Z2";Barra_1="X607992";Barra_2="X607992";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00543";TIPO="Não Definida";Circuito_1="FDR-01J1";Circuito_2="FDR-01Z3";Barra_1="9662826-1";Barra_2="9662826-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F38013";TIPO="Chave Fusível";Circuito_1="FDR-01J2";Circuito_2="FDR-01J2";Barra_1="X111901";Barra_2="165743-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F02557";TIPO="Seccionalizadora";Circuito_1="FDR-01J2";Circuito_2="FDR-01J2";Barra_1="12253239-1";Barra_2="12253239-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90711";TIPO="Não Definida";Circuito_1="FDR-01J2";Circuito_2="FDR-01Z2";Barra_1="X116183";Barra_2="X116183";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F02281";TIPO="Não Definida";Circuito_1="FDR-01J2";Circuito_2="FDR-01Z2";Barra_1="X112132";Barra_2="X112132";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00302";TIPO="Não Definida";Circuito_1="FDR-01J2";Circuito_2="GRC-01L1";Barra_1="X099643";Barra_2="X099643";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53204";TIPO="Não Definida";Circuito_1="FDR-01J2";Circuito_2="GRC-01L1";Barra_1="X116123";Barra_2="X116123";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04040";TIPO="Não Definida";Circuito_1="FDR-01J3";Circuito_2="FDR-01J8";Barra_1="X112162";Barra_2="X112162";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="X12612";TIPO="Não Definida";Circuito_1="FDR-01J3";Circuito_2="FDR-01J5";Barra_1="172568-1";Barra_2="X112718";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51307";TIPO="Não Definida";Circuito_1="FDR-01J3";Circuito_2="FDR-01J8";Barra_1="X516949";Barra_2="X516949";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52175";TIPO="Não Definida";Circuito_1="FDR-01J3";Circuito_2="FDR-01J8";Barra_1="X561845";Barra_2="X561845";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00025";TIPO="Não Definida";Circuito_1="FDR-01J4";Circuito_2="FDR-01Z3";Barra_1="X063048";Barra_2="X063048";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90029";TIPO="Não Definida";Circuito_1="FDR-01J4";Circuito_2="FDR-01J1";Barra_1="172093-1";Barra_2="X111783";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00180";TIPO="Não Definida";Circuito_1="FDR-01J4";Circuito_2="FDR-01Z2";Barra_1="X114536";Barra_2="X114536";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00381";TIPO="Não Definida";Circuito_1="FDR-01J4";Circuito_2="CDL-01P7";Barra_1="X065777";Barra_2="X065777";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00306";TIPO="Não Definida";Circuito_1="FDR-01J5";Circuito_2="FDR-01J4";Barra_1="X113054";Barra_2="X113054";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F12610";TIPO="Não Definida";Circuito_1="FDR-01J5";Circuito_2="FDR-01J3";Barra_1="X112718";Barra_2="172568-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57100";TIPO="Não Definida";Circuito_1="FDR-01J5";Circuito_2="FDR-01Z3";Barra_1="X114953";Barra_2="X114953";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00137";TIPO="Não Definida";Circuito_1="FDR-01J5";Circuito_2="FNV-01V3";Barra_1="X115511";Barra_2="X115511";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00070";TIPO="Não Definida";Circuito_1="FDR-01J6";Circuito_2="CEN-01M1";Barra_1="X114806";Barra_2="X114806";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00034";TIPO="Não Definida";Circuito_1="FDR-01J6";Circuito_2="FDR-01J3";Barra_1="9765213-1";Barra_2="9765213-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00117";TIPO="Não Definida";Circuito_1="FDR-01J6";Circuito_2="FDR-01Z1";Barra_1="X112995";Barra_2="X112995";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56406";TIPO="Não Definida";Circuito_1="FDR-01J6";Circuito_2="FDR-01Z4";Barra_1="X114885";Barra_2="X114885";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00384";TIPO="Não Definida";Circuito_1="FDR-01J7";Circuito_2="FDR-01J4";Barra_1="X112658";Barra_2="X112658";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00196";TIPO="Não Definida";Circuito_1="FDR-01J7";Circuito_2="GRC-01Y1";Barra_1="X115099";Barra_2="X115099";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00074";TIPO="Não Definida";Circuito_1="FDR-01J7";Circuito_2="GRC-01Y7";Barra_1="X115468";Barra_2="X115468";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00037";TIPO="Não Definida";Circuito_1="FDR-01J8";Circuito_2="FDR-01J5";Barra_1="171760-2";Barra_2="X114821";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00165";TIPO="Não Definida";Circuito_1="FDR-01J8";Circuito_2="FNV-01V3";Barra_1="9662813-1";Barra_2="9662813-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00121";TIPO="Não Definida";Circuito_1="FDR-01Z1";Circuito_2="CEN-01J4";Barra_1="X135914";Barra_2="X135914";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54944";TIPO="Não Definida";Circuito_1="FDR-01Z1";Circuito_2="FDR-01Z4";Barra_1="X114282";Barra_2="X114282";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00212";TIPO="Não Definida";Circuito_1="FDR-01Z2";Circuito_2="FDR-01J2";Barra_1="104725565-1";Barra_2="104725565-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00448";TIPO="Não Definida";Circuito_1="FDR-01Z2";Circuito_2="AML-01X2";Barra_1="X056189";Barra_2="X056189";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53432";TIPO="Não Definida";Circuito_1="FDR-01Z2";Circuito_2="FDR-01J4";Barra_1="X112016";Barra_2="X112016";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00519";TIPO="Não Definida";Circuito_1="FDR-01Z3";Circuito_2="FNV-01F1";Barra_1="X101051";Barra_2="X101051";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00277";TIPO="Não Definida";Circuito_1="FDR-01Z3";Circuito_2="FNV-01F1";Barra_1="X113648";Barra_2="X113648";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00218";TIPO="Não Definida";Circuito_1="FDR-01Z3";Circuito_2="MTT-01L3";Barra_1="X101488";Barra_2="X101488";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00394";TIPO="Não Definida";Circuito_1="FDR-01Z4";Circuito_2="CEN-01X4";Barra_1="X074318";Barra_2="X074318";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26841";TIPO="Seccionalizadora";Circuito_1="FNV-01F1";Circuito_2="FNV-01F1";Barra_1="X801973";Barra_2="510356-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F29893";TIPO="Não Definida";Circuito_1="FNV-01F1";Circuito_2="FDR-01Z3";Barra_1="104725843-1";Barra_2="104725843-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00282";TIPO="Não Definida";Circuito_1="FNV-01F1";Circuito_2="FDR-01J3";Barra_1="X112486";Barra_2="X112486";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51624";TIPO="Não Definida";Circuito_1="FNV-01F1";Circuito_2="FNV-01V4";Barra_1="12495862-1";Barra_2="X517108";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51668";TIPO="Não Definida";Circuito_1="FNV-01F1";Circuito_2="FNV-01V4";Barra_1="X469193";Barra_2="X469193";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52307";TIPO="Não Definida";Circuito_1="FNV-01F1";Circuito_2="MTT-01L2";Barra_1="X108644";Barra_2="X108644";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00018";TIPO="Não Definida";Circuito_1="FNV-01F5";Circuito_2="FDR-01Z1";Barra_1="9663016-1";Barra_2="X116411";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00525";TIPO="Não Definida";Circuito_1="FNV-01F5";Circuito_2="FNV-01V3";Barra_1="X117065";Barra_2="X117065";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52957";TIPO="Não Definida";Circuito_1="FNV-01F5";Circuito_2="CEN-01J5";Barra_1="X111497";Barra_2="X111497";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F01453";TIPO="Não Definida";Circuito_1="FNV-01F5";Circuito_2="MTT-01N6";Barra_1="104725851-1";Barra_2="104725851-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00434";TIPO="Não Definida";Circuito_1="FNV-01V1";Circuito_2="CEN-01M3";Barra_1="X111168";Barra_2="X111168";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51368";TIPO="Não Definida";Circuito_1="FNV-01V1";Circuito_2="MTT-01N6";Barra_1="X111363";Barra_2="X111363";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00758";TIPO="Não Definida";Circuito_1="FNV-01V1";Circuito_2="MTT-01N6";Barra_1="X111342";Barra_2="X111342";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00388";TIPO="Não Definida";Circuito_1="FNV-01V2";Circuito_2="CEN-01M2";Barra_1="X111727";Barra_2="X111727";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F01536";TIPO="Não Definida";Circuito_1="FNV-01V2";Circuito_2="CEN-01M3";Barra_1="104725852-1";Barra_2="104725852-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53015";TIPO="Não Definida";Circuito_1="FNV-01V3";Circuito_2="FNV-01V4";Barra_1="X102943";Barra_2="X102943";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00010";TIPO="Não Definida";Circuito_1="FNV-01V4";Circuito_2="FNV-01F5";Barra_1="X102578";Barra_2="X102578";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50075";TIPO="Não Definida";Circuito_1="FNV-01V4";Circuito_2="FDR-01J5";Barra_1="X103549";Barra_2="X103549";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00522";TIPO="Não Definida";Circuito_1="FNV-01V4";Circuito_2="MTT-01L1";Barra_1="X101195";Barra_2="X101195";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34701";TIPO="Seccionalizadora";Circuito_1="GBA-01F1";Circuito_2="GBA-01F1";Barra_1="9860601-1";Barra_2="9860601-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00472";TIPO="Não Definida";Circuito_1="GBA-01F2";Circuito_2="GBA-01F3";Barra_1="9655106-1";Barra_2="9655106-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F40125";TIPO="Não Definida";Circuito_1="GBA-01F2";Circuito_2="GBA-01F4";Barra_1="X280113";Barra_2="X280113";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00473";TIPO="Não Definida";Circuito_1="GBA-01F3";Circuito_2="GBA-01F1";Barra_1="X188020";Barra_2="X188020";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F41864";TIPO="Não Definida";Circuito_1="GBA-01F4";Circuito_2="GBA-01F2";Barra_1="X602554";Barra_2="X602554";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53458";TIPO="Não Definida";Circuito_1="GBA-01F5";Circuito_2="GBA-01F6";Barra_1="X602392";Barra_2="X602392";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00496";TIPO="Não Definida";Circuito_1="GBA-01F5";Circuito_2="GBA-01F4";Barra_1="12740324-1";Barra_2="12740324-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53900";TIPO="Não Definida";Circuito_1="GBA-01F6";Circuito_2="GBA-01F1";Barra_1="X188656";Barra_2="X188656";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00454";TIPO="Não Definida";Circuito_1="GBA-01F6";Circuito_2="ARB-01V4";Barra_1="X190024";Barra_2="X190024";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49804";TIPO="Não Definida";Circuito_1="GRC-01L1";Circuito_2="GRC-01L6";Barra_1="150843-1";Barra_2="X067833";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56569";TIPO="Não Definida";Circuito_1="GRC-01L1";Circuito_2="FDR-01J7";Barra_1="X115606";Barra_2="X115606";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50663";TIPO="Não Definida";Circuito_1="GRC-01L2";Circuito_2="GRC-01L1";Barra_1="12748062-1";Barra_2="12748062-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00209";TIPO="Não Definida";Circuito_1="GRC-01L2";Circuito_2="GRC-01L1";Barra_1="X066579";Barra_2="X066579";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00079";TIPO="Não Definida";Circuito_1="GRC-01L3";Circuito_2="GRC-01Y7";Barra_1="150779-1";Barra_2="X067304";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00237";TIPO="Não Definida";Circuito_1="GRC-01L3";Circuito_2="GRC-01L2";Barra_1="X070000";Barra_2="X070000";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00061";TIPO="Não Definida";Circuito_1="GRC-01L3";Circuito_2="GRC-01L4";Barra_1="X069837";Barra_2="X069837";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00730";TIPO="Não Definida";Circuito_1="GRC-01L3";Circuito_2="GRC-01L4";Barra_1="X461366";Barra_2="X461366";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00099";TIPO="Não Definida";Circuito_1="GRC-01L6";Circuito_2="GRC-01Y2";Barra_1="X071065";Barra_2="X071065";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00307";TIPO="Não Definida";Circuito_1="GRC-01L6";Circuito_2="GRC-01L4";Barra_1="X467100";Barra_2="X467100";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F15059";TIPO="Chave Fusível";Circuito_1="GRC-01Y2";Circuito_2="GRC-01Y2";Barra_1="104725858-1";Barra_2="104725858-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00329";TIPO="Não Definida";Circuito_1="GRC-01Y2";Circuito_2="GRC-01Y1";Barra_1="X071401";Barra_2="X071401";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00094";TIPO="Não Definida";Circuito_1="GRC-01Y2";Circuito_2="GRC-01Y5";Barra_1="X068488";Barra_2="X068488";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00163";TIPO="Não Definida";Circuito_1="GRC-01Y2";Circuito_2="GRC-01Y1";Barra_1="X071288";Barra_2="X071288";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50442";TIPO="Não Definida";Circuito_1="GRC-01Y5";Circuito_2="GRC-01Y7";Barra_1="X499888";Barra_2="X499888";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F47988";TIPO="Não Definida";Circuito_1="GRC-01Y5";Circuito_2="FDR-01Z4";Barra_1="12333961-1";Barra_2="12333961-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00427";TIPO="Não Definida";Circuito_1="GRC-01Y5";Circuito_2="GRC-01Y1";Barra_1="X067423";Barra_2="X067423";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00049";TIPO="Não Definida";Circuito_1="GRC-01Y6";Circuito_2="GRC-01Y2";Barra_1="9443107-1";Barra_2="9443107-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50129";TIPO="Não Definida";Circuito_1="GRC-01Y6";Circuito_2="GRC-01L6";Barra_1="X496252";Barra_2="X496252";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00362";TIPO="Não Definida";Circuito_1="GRC-01Y6";Circuito_2="GRC-01L4";Barra_1="X073555";Barra_2="X073555";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00393";TIPO="Não Definida";Circuito_1="GRC-01Y7";Circuito_2="GRC-01Y6";Barra_1="9649915-1";Barra_2="9649915-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00304";TIPO="Não Definida";Circuito_1="GRC-01Y7";Circuito_2="FDR-01J3";Barra_1="12090642-1";Barra_2="12090642-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00389";TIPO="Não Definida";Circuito_1="GRC-01Y7";Circuito_2="CEN-01X4";Barra_1="X117551";Barra_2="X117551";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00107";TIPO="Não Definida";Circuito_1="GRC-01Y7";Circuito_2="FDR-01J3";Barra_1="9214448-1";Barra_2="9214448-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00351";TIPO="Não Definida";Circuito_1="IIN-01F1";Circuito_2="SCR-01J3";Barra_1="X210059";Barra_2="X210059";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56834";TIPO="Não Definida";Circuito_1="IIN-01F1";Circuito_2="SCR-01J3";Barra_1="X168629";Barra_2="X168629";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24617";TIPO="Seccionalizadora";Circuito_1="IIN-01F2";Circuito_2="IIN-01F2";Barra_1="X501783";Barra_2="662847-1_AUX2";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24599";TIPO="Não Definida";Circuito_1="IIN-01F2";Circuito_2="SCR-01J1";Barra_1="X501725";Barra_2="X501725";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00200";TIPO="Não Definida";Circuito_1="IIN-01F2";Circuito_2="SCR-01N6";Barra_1="X210087";Barra_2="X210087";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52298";TIPO="Não Definida";Circuito_1="IIN-01F3";Circuito_2="IIN-01F2";Barra_1="X035455";Barra_2="X035455";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56601";TIPO="Não Definida";Circuito_1="IIN-01F3";Circuito_2="LDF-01M7";Barra_1="X026296";Barra_2="X026296";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56555";TIPO="Não Definida";Circuito_1="IIN-01F4";Circuito_2="IIN-01F1";Barra_1="X459403";Barra_2="X459403";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00724";TIPO="Não Definida";Circuito_1="IIN-01F4";Circuito_2="IIN-01F5";Barra_1="X040941";Barra_2="X040941";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90555";TIPO="Não Definida";Circuito_1="IIN-01F4";Circuito_2="LDF-01L2";Barra_1="X048696";Barra_2="X048696";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00333";TIPO="Não Definida";Circuito_1="IIN-01F5";Circuito_2="SCR-01J4";Barra_1="X040496";Barra_2="X040496";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50275";TIPO="Não Definida";Circuito_1="IMB-01J1";Circuito_2="CAB-01S3";Barra_1="X001965";Barra_2="X001965";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00751";TIPO="Não Definida";Circuito_1="IMB-01J1";Circuito_2="IMB-01J3";Barra_1="X012011";Barra_2="X012011";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54333";TIPO="Não Definida";Circuito_1="IMB-01J1";Circuito_2="IMB-01J3";Barra_1="X012250";Barra_2="X012250";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00752";TIPO="Não Definida";Circuito_1="IMB-01J1";Circuito_2="CAB-01C6";Barra_1="X002001";Barra_2="X002001";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90113";TIPO="Não Definida";Circuito_1="IMB-01J1";Circuito_2="PIT-01Y1";Barra_1="X010599";Barra_2="X010599";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54327";TIPO="Não Definida";Circuito_1="IMB-01J2";Circuito_2="PUD-01P5";Barra_1="X012789";Barra_2="X012789";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34711";TIPO="Seccionalizadora";Circuito_1="IMB-01J3";Circuito_2="IMB-01J3";Barra_1="104555221-1";Barra_2="104555221-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F48100";TIPO="Não Definida";Circuito_1="IMB-01J3";Circuito_2="IMB-01J1";Barra_1="X407692";Barra_2="X407692";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51956";TIPO="Não Definida";Circuito_1="IMB-01J3";Circuito_2="IMB-01J1";Barra_1="X564182";Barra_2="X564182";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00120";TIPO="Não Definida";Circuito_1="IMB-01J3";Circuito_2="IMB-01J2";Barra_1="X010185";Barra_2="X010185";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51333";TIPO="Não Definida";Circuito_1="IMB-01J3";Circuito_2="CAB-01C1";Barra_1="X008894";Barra_2="X008894";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00062";TIPO="Não Definida";Circuito_1="IPG-01C3";Circuito_2="IPG-01C5";Barra_1="X123980";Barra_2="X123879";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00146";TIPO="Não Definida";Circuito_1="IPG-01C3";Circuito_2="LPN-01M4";Barra_1="X123980";Barra_2="X123980";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00686";TIPO="Não Definida";Circuito_1="IPG-01C3";Circuito_2="LPN-01M6";Barra_1="X123258";Barra_2="X123258";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00114";TIPO="Não Definida";Circuito_1="IPG-01C4";Circuito_2="IPG-01C5";Barra_1="147096-1";Barra_2="X138816";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00135";TIPO="Não Definida";Circuito_1="IPG-01C5";Circuito_2="IPG-01C2";Barra_1="9442911-1";Barra_2="9442911-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00577";TIPO="Não Definida";Circuito_1="IPG-01Z1";Circuito_2="IPG-01Z2";Barra_1="X134927";Barra_2="X134927";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90489";TIPO="Chave Fusível";Circuito_1="IPG-01Z2";Circuito_2="IPG-01Z2";Barra_1="X141436";Barra_2="141402-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56553";TIPO="Não Definida";Circuito_1="IPG-01Z2";Circuito_2="IPG-01Z3";Barra_1="9501132-1";Barra_2="9501132-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00246";TIPO="Não Definida";Circuito_1="IPG-01Z2";Circuito_2="IPG-01Z3";Barra_1="X132831";Barra_2="X132831";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00039";TIPO="Não Definida";Circuito_1="IPG-01Z2";Circuito_2="IPG-01Z4";Barra_1="X134758";Barra_2="142532-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90009";TIPO="Não Definida";Circuito_1="IPG-01Z2";Circuito_2="IPG-01Z3";Barra_1="X140840";Barra_2="X140840";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00456";TIPO="Não Definida";Circuito_1="IPG-01Z3";Circuito_2="IPG-01C3";Barra_1="X132871";Barra_2="X132871";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00581";TIPO="Não Definida";Circuito_1="IPG-01Z3";Circuito_2="IPG-01C3";Barra_1="X132247";Barra_2="X132247";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00305";TIPO="Não Definida";Circuito_1="IPG-01Z3";Circuito_2="IPG-01Z4";Barra_1="X137484";Barra_2="X137484";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90056";TIPO="Não Definida";Circuito_1="IPG-01Z3";Circuito_2="IPG-01C3";Barra_1="X133803";Barra_2="X133803";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00606";TIPO="Não Definida";Circuito_1="IPG-01Z4";Circuito_2="IPG-01Z5";Barra_1="9533245-1";Barra_2="9533245-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00311";TIPO="Não Definida";Circuito_1="IPG-01Z5";Circuito_2="IPG-01C3";Barra_1="9655088-1";Barra_2="9655088-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00080";TIPO="Não Definida";Circuito_1="IPG-01Z5";Circuito_2="IPG-01C3";Barra_1="X123898";Barra_2="X123898";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04916";TIPO="Não Definida";Circuito_1="IPG-01Z5";Circuito_2="IPG-01Z4";Barra_1="12463842-1";Barra_2="X138580";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00316";TIPO="Não Definida";Circuito_1="IPG-01Z5";Circuito_2="IPG-01Z4";Barra_1="X197892";Barra_2="X197892";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53886";TIPO="Não Definida";Circuito_1="IPG-01Z5";Circuito_2="LPN-01M6";Barra_1="12731374-1";Barra_2="12731374-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F91111";TIPO="Não Definida";Circuito_1="LBT-01S1";Circuito_2="LBT-01S2";Barra_1="145368-1";Barra_2="X140399";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F36553";TIPO="Não Definida";Circuito_1="LBT-01S3";Circuito_2="LBT-01S4";Barra_1="X134614";Barra_2="X134614";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00071";TIPO="Não Definida";Circuito_1="LBT-01S3";Circuito_2="CJD-01F5";Barra_1="X153928";Barra_2="X153928";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56251";TIPO="Seccionalizadora";Circuito_1="LBT-01S4";Circuito_2="LBT-01S4";Barra_1="X140051";Barra_2="138350-2_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54393";TIPO="Chave Fusível";Circuito_1="LBT-01S4";Circuito_2="LBT-01S4";Barra_1="X137289";Barra_2="139979-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F26167";TIPO="Não Definida";Circuito_1="LDF-01L1";Circuito_2="LDF-01L3";Barra_1="X021454";Barra_2="X021454";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="M00338";TIPO="Não Definida";Circuito_1="LDF-01L1";Circuito_2="LDF-01M8";Barra_1="X022209";Barra_2="X022209";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F48524";TIPO="Não Definida";Circuito_1="LDF-01L1";Circuito_2="LDF-01M5";Barra_1="P530941";Barra_2="P530941";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50744";TIPO="Não Definida";Circuito_1="LDF-01L2";Circuito_2="LDF-01M1";Barra_1="X043490";Barra_2="X043490";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51422";TIPO="Não Definida";Circuito_1="LDF-01L2";Circuito_2="IIN-01F4";Barra_1="X044807";Barra_2="X044807";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50740";TIPO="Não Definida";Circuito_1="LDF-01L2";Circuito_2="IIN-01F5";Barra_1="X043393";Barra_2="X043393";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00243";TIPO="Não Definida";Circuito_1="LDF-01L3";Circuito_2="LDF-01M3";Barra_1="273996-1";Barra_2="X019314";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00417";TIPO="Não Definida";Circuito_1="LDF-01L3";Circuito_2="LDF-01M6";Barra_1="P530943";Barra_2="P530943";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00647";TIPO="Não Definida";Circuito_1="LDF-01M1";Circuito_2="LDF-01M7";Barra_1="X414650";Barra_2="X414650";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00299";TIPO="Não Definida";Circuito_1="LDF-01M1";Circuito_2="IIN-01F5";Barra_1="X043399";Barra_2="X043399";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00422";TIPO="Não Definida";Circuito_1="LDF-01M1";Circuito_2="LDF-01M3";Barra_1="104725637-1";Barra_2="104725637-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00149";TIPO="Não Definida";Circuito_1="LDF-01M1";Circuito_2="LDF-01M7";Barra_1="X040904";Barra_2="X040904";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34792";TIPO="Seccionalizadora";Circuito_1="LDF-01M2";Circuito_2="LDF-01M2";Barra_1="10007991-1";Barra_2="10007991-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49099";TIPO="Não Definida";Circuito_1="LDF-01M2";Circuito_2="LDF-01M8";Barra_1="X021532";Barra_2="X021532";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49982";TIPO="Não Definida";Circuito_1="LDF-01M2";Circuito_2="LDF-01L1";Barra_1="X037646";Barra_2="X037646";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00354";TIPO="Não Definida";Circuito_1="LDF-01M2";Circuito_2="LDF-01M6";Barra_1="X038001";Barra_2="X038001";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00303";TIPO="Não Definida";Circuito_1="LDF-01M2";Circuito_2="LDF-01M7";Barra_1="X501536";Barra_2="X501536";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F46363";TIPO="Seccionalizadora";Circuito_1="LDF-01M4";Circuito_2="LDF-01M4";Barra_1="X033086";Barra_2="64835-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50344";TIPO="Não Definida";Circuito_1="LDF-01M5";Circuito_2="LDF-01L3";Barra_1="12437683-1";Barra_2="12437683-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00755";TIPO="Não Definida";Circuito_1="LDF-01M5";Circuito_2="LDF-01M4";Barra_1="X481945";Barra_2="X481945";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53305";TIPO="Não Definida";Circuito_1="LDF-01M5";Circuito_2="LDF-01M4";Barra_1="67538-1";Barra_2="X030788";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00412";TIPO="Não Definida";Circuito_1="LDF-01M6";Circuito_2="LDF-01M2";Barra_1="X038388";Barra_2="X038388";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56529";TIPO="Não Definida";Circuito_1="LDF-01M6";Circuito_2="LDF-01M9";Barra_1="12843042-1";Barra_2="12843042-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00050";TIPO="Não Definida";Circuito_1="LDF-01M7";Circuito_2="IIN-01F3";Barra_1="X036129";Barra_2="X036129";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00283";TIPO="Não Definida";Circuito_1="LDF-01M7";Circuito_2="IIN-01F5";Barra_1="X030047";Barra_2="X030047";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00370";TIPO="Não Definida";Circuito_1="LDF-01M7";Circuito_2="LDF-01M8";Barra_1="X043712";Barra_2="X043712";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00201";TIPO="Não Definida";Circuito_1="LDF-01M7";Circuito_2="LDF-01M8";Barra_1="71186-1";Barra_2="X021339";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00539";TIPO="Não Definida";Circuito_1="LDF-01M8";Circuito_2="IIN-01F3";Barra_1="X026734";Barra_2="X026734";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00536";TIPO="Não Definida";Circuito_1="LDF-01M8";Circuito_2="SCR-01N5";Barra_1="X026499";Barra_2="X026499";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F44012";TIPO="Chave Fusível";Circuito_1="LDF-01M9";Circuito_2="LDF-01M9";Barra_1="X034401";Barra_2="65075-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00095";TIPO="Não Definida";Circuito_1="LDF-01M9";Circuito_2="LDF-01M4";Barra_1="X035628";Barra_2="X035628";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00555";TIPO="Não Definida";Circuito_1="LDF-01M9";Circuito_2="ARB-01V1";Barra_1="X052812";Barra_2="X052812";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49095";TIPO="Não Definida";Circuito_1="LDF-01M9";Circuito_2="LDF-01L1";Barra_1="X037299";Barra_2="X037299";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00595";TIPO="Não Definida";Circuito_1="LDF-01M9";Circuito_2="LDF-01M3";Barra_1="X501541";Barra_2="X501541";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00754";TIPO="Não Definida";Circuito_1="LDF-01M9";Circuito_2="LDF-01M5";Barra_1="X037225";Barra_2="X037225";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00067";TIPO="Não Definida";Circuito_1="LPN-01M1";Circuito_2="LPN-01M5";Barra_1="X119938";Barra_2="X119938";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00431";TIPO="Não Definida";Circuito_1="LPN-01M1";Circuito_2="LPN-01M5";Barra_1="156452-1";Barra_2="X120504";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00097";TIPO="Não Definida";Circuito_1="LPN-01M2";Circuito_2="LPN-01M1";Barra_1="X119913";Barra_2="X119913";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F20666";TIPO="Não Definida";Circuito_1="LPN-01M3";Circuito_2="FNV-01V1";Barra_1="12655979-1";Barra_2="12655979-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00723";TIPO="Não Definida";Circuito_1="LPN-01M3";Circuito_2="FNV-01V1";Barra_1="X123469";Barra_2="X123469";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00385";TIPO="Não Definida";Circuito_1="LPN-01M4";Circuito_2="IPG-01C4";Barra_1="X121359";Barra_2="X121359";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00104";TIPO="Não Definida";Circuito_1="LPN-01M4";Circuito_2="IPG-01Z5";Barra_1="X140256";Barra_2="X140256";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F13032";TIPO="Chave Fusível";Circuito_1="LPN-01M5";Circuito_2="LPN-01M5";Barra_1="X119982";Barra_2="156691-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04848";TIPO="Seccionalizadora";Circuito_1="LPN-01M5";Circuito_2="LPN-01M5";Barra_1="X120712";Barra_2="156523-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00132";TIPO="Não Definida";Circuito_1="LPN-01M5";Circuito_2="LPN-01M2";Barra_1="9627425-1";Barra_2="9627425-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52271";TIPO="Não Definida";Circuito_1="LPN-01M5";Circuito_2="LPN-01M4";Barra_1="X568445";Barra_2="X568445";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00125";TIPO="Não Definida";Circuito_1="LPN-01M6";Circuito_2="LPN-01M5";Barra_1="X120532";Barra_2="X120532";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00056";TIPO="Não Definida";Circuito_1="LPN-01M6";Circuito_2="IPG-01C3";Barra_1="X611840";Barra_2="X611840";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53713";TIPO="Não Definida";Circuito_1="LPN-01M6";Circuito_2="IPG-01Z5";Barra_1="X606549";Barra_2="X606549";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00219";TIPO="Não Definida";Circuito_1="LPN-01M7";Circuito_2="LPN-01M3";Barra_1="X119610";Barra_2="X119610";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00369";TIPO="Não Definida";Circuito_1="LPN-01M7";Circuito_2="LPN-01M4";Barra_1="X120749";Barra_2="X120749";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00127";TIPO="Não Definida";Circuito_1="LPN-01M8";Circuito_2="LPN-01M4";Barra_1="104295022-1";Barra_2="104295022-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00077";TIPO="Não Definida";Circuito_1="LPN-01M8";Circuito_2="LPN-01M4";Barra_1="X121873";Barra_2="X121873";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00551";TIPO="Não Definida";Circuito_1="MTR-01X1";Circuito_2="MTR-01X3";Barra_1="X092491";Barra_2="X092491";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52156";TIPO="Seccionalizadora";Circuito_1="MTR-01X3";Circuito_2="MTR-01X3";Barra_1="12634769-1";Barra_2="12634769-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52199";TIPO="Chave a óleo";Circuito_1="MTR-01X3";Circuito_2="MTR-01X3";Barra_1="X207652";Barra_2="178355-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00649";TIPO="Não Definida";Circuito_1="MTR-01X5";Circuito_2="MTR-01X6";Barra_1="104187326-1";Barra_2="104187326-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00594";TIPO="Não Definida";Circuito_1="MTR-01X6";Circuito_2="MTR-01X7";Barra_1="X096769";Barra_2="X096769";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00365";TIPO="Não Definida";Circuito_1="MTT-01L1";Circuito_2="FNV-01V4";Barra_1="X101114";Barra_2="X101114";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00364";TIPO="Não Definida";Circuito_1="MTT-01L1";Circuito_2="FNV-01F1";Barra_1="X101123";Barra_2="X101123";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00029";TIPO="Não Definida";Circuito_1="MTT-01L3";Circuito_2="CDL-01P4";Barra_1="X102020";Barra_2="X102020";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00016";TIPO="Não Definida";Circuito_1="MTT-01L4";Circuito_2="MTT-01L3";Barra_1="X101832";Barra_2="X101832";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00402";TIPO="Não Definida";Circuito_1="MTT-01L6";Circuito_2="RET-01Y4";Barra_1="X107024";Barra_2="X107024";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54453";TIPO="Não Definida";Circuito_1="MTT-01N1";Circuito_2="MTT-01N4";Barra_1="X110182";Barra_2="X110182";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00759";TIPO="Não Definida";Circuito_1="MTT-01N1";Circuito_2="MTT-01N5";Barra_1="10212269-1";Barra_2="10212269-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00279";TIPO="Não Definida";Circuito_1="MTT-01N1";Circuito_2="MTT-01N3";Barra_1="X108991";Barra_2="X108991";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53841";TIPO="Não Definida";Circuito_1="MTT-01N2";Circuito_2="MTT-01N3";Barra_1="X108659";Barra_2="X108659";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00009";TIPO="Não Definida";Circuito_1="MTT-01N2";Circuito_2="LPN-01M3";Barra_1="X108306";Barra_2="X108306";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00766";TIPO="Não Definida";Circuito_1="MTT-01N2";Circuito_2="RET-01Y2";Barra_1="X107189";Barra_2="12060668-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00238";TIPO="Não Definida";Circuito_1="MTT-01N3";Circuito_2="MTT-01N2";Barra_1="X109008";Barra_2="X109008";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00765";TIPO="Não Definida";Circuito_1="MTT-01N4";Circuito_2="MTT-01N6";Barra_1="X102685";Barra_2="X102685";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F47958";TIPO="Não Definida";Circuito_1="MTT-01N4";Circuito_2="LPN-01M3";Barra_1="X487860";Barra_2="X487860";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F23724";TIPO="Não Definida";Circuito_1="MTT-01N4";Circuito_2="MTT-01N3";Barra_1="X432313";Barra_2="X432313";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00392";TIPO="Não Definida";Circuito_1="MTT-01N5";Circuito_2="CEN-01M3";Barra_1="104725636-1";Barra_2="104725636-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F31833";TIPO="Não Definida";Circuito_1="MTT-01N5";Circuito_2="CEN-01M3";Barra_1="9654993-1";Barra_2="9654993-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00001";TIPO="Não Definida";Circuito_1="MTT-01N5";Circuito_2="LPN-01M1";Barra_1="9526529-1";Barra_2="9526529-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00770";TIPO="Não Definida";Circuito_1="MTT-01N6";Circuito_2="MTT-01N5";Barra_1="X111307";Barra_2="X111307";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52932";TIPO="Não Definida";Circuito_1="MTT-01N6";Circuito_2="FNV-01V1";Barra_1="X110598";Barra_2="X110598";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F27369";TIPO="Seccionalizadora";Circuito_1="PIT-01M1";Circuito_2="PIT-01M1";Barra_1="8655641-1";Barra_2="8655641-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00408";TIPO="Não Definida";Circuito_1="PIT-01M1";Circuito_2="AML-01S3";Barra_1="X022727";Barra_2="X022723";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00377";TIPO="Não Definida";Circuito_1="PIT-01M1";Circuito_2="AML-01S2";Barra_1="X501262";Barra_2="X501262";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="300604";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="AML-01S3";Barra_1="X023088";Barra_2="X023088";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00123";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01M1";Barra_1="X023340";Barra_2="X023340";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F21963";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01M1";Barra_1="104187396-1";Barra_2="104187396-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00541";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01M1";Barra_1="X500219";Barra_2="X500219";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00181";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01M1";Barra_1="X501216";Barra_2="X501216";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00509";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01Y4";Barra_1="X025056";Barra_2="X025056";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F12178";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01Y4";Barra_1="X025196";Barra_2="X025196";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00517";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01Y5";Barra_1="X488983";Barra_2="X488983";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00376";TIPO="Não Definida";Circuito_1="PIT-01M2";Circuito_2="PIT-01Y5";Barra_1="117254-1";Barra_2="X501217";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90059";TIPO="Chave Fusível";Circuito_1="PIT-01Y1";Circuito_2="PIT-01Y1";Barra_1="X010592";Barra_2="109663-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57252";TIPO="Chave Fusível";Circuito_1="PIT-01Y1";Circuito_2="PIT-01Y1";Barra_1="X617717";Barra_2="12900377-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56456";TIPO="Não Definida";Circuito_1="PIT-01Y1";Circuito_2="CDL-01W7";Barra_1="X001235";Barra_2="X001235";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90108";TIPO="Não Definida";Circuito_1="PIT-01Y1";Circuito_2="IMB-01J2";Barra_1="X023063";Barra_2="X023063";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00691";TIPO="Não Definida";Circuito_1="PIT-01Y1";Circuito_2="IMB-01J3";Barra_1="X010788";Barra_2="X010788";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00438";TIPO="Não Definida";Circuito_1="PIT-01Y1";Circuito_2="PIT-01Y8";Barra_1="12597918-1";Barra_2="12597918-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F36831";TIPO="Seccionalizadora";Circuito_1="PIT-01Y2";Circuito_2="PIT-01Y2";Barra_1="12122350-1";Barra_2="12122350-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00540";TIPO="Não Definida";Circuito_1="PIT-01Y2";Circuito_2="PIT-01Y3";Barra_1="DX500276";Barra_2="DX500276";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00531";TIPO="Não Definida";Circuito_1="PIT-01Y2";Circuito_2="PIT-01Y3";Barra_1="X028071";Barra_2="X028071";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00444";TIPO="Não Definida";Circuito_1="PIT-01Y2";Circuito_2="CDL-01W5";Barra_1="X032079";Barra_2="X032079";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F10176";TIPO="Não Definida";Circuito_1="PIT-01Y3";Circuito_2="CDL-01P5";Barra_1="104725856-1";Barra_2="104725856-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00134";TIPO="Não Definida";Circuito_1="PIT-01Y3";Circuito_2="CDL-01P5";Barra_1="X029174";Barra_2="X029174";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00102";TIPO="Não Definida";Circuito_1="PIT-01Y3";Circuito_2="PIT-01Y4";Barra_1="X028164";Barra_2="X028164";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00332";TIPO="Não Definida";Circuito_1="PIT-01Y4";Circuito_2="PIT-01Y5";Barra_1="X025408";Barra_2="X025408";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00685";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="AML-01S3";Barra_1="X022654";Barra_2="X022654";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00089";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="CDL-01W3";Barra_1="X032803";Barra_2="X032803";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F20668";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="PIT-01Y4";Barra_1="104725652-1";Barra_2="104725652-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00183";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="PIT-01Y4";Barra_1="X032152";Barra_2="X032152";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00533";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="PIT-01Y4";Barra_1="X025538";Barra_2="X025538";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F36340";TIPO="Não Definida";Circuito_1="PIT-01Y5";Circuito_2="AML-01S3";Barra_1="X457452";Barra_2="X457452";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24834";TIPO="Não Definida";Circuito_1="PIT-01Y6";Circuito_2="PIT-01Y1";Barra_1="X030925";Barra_2="X030925";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00105";TIPO="Não Definida";Circuito_1="PIT-01Y6";Circuito_2="PIT-01Y1";Barra_1="898940-1";Barra_2="898940-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00036";TIPO="Não Definida";Circuito_1="PIT-01Y6";Circuito_2="PIT-01Y1";Barra_1="X023403";Barra_2="X023403";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00407";TIPO="Não Definida";Circuito_1="PIT-01Y7";Circuito_2="PIT-01Y1";Barra_1="X029840";Barra_2="X029840";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24166";TIPO="Não Definida";Circuito_1="PIT-01Y7";Circuito_2="PIT-01Y1";Barra_1="X501490";Barra_2="X501490";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00416";TIPO="Não Definida";Circuito_1="PIT-01Y7";Circuito_2="PIT-01Y6";Barra_1="X022876";Barra_2="X022876";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00154";TIPO="Não Definida";Circuito_1="PIT-01Y8";Circuito_2="PIT-01Y7";Barra_1="X028449";Barra_2="X028449";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00588";TIPO="Não Definida";Circuito_1="PPE-01Y1";Circuito_2="PPE-01Y3";Barra_1="9214714-1";Barra_2="9214714-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00583";TIPO="Não Definida";Circuito_1="PPE-01Y2";Circuito_2="PPE-01Y3";Barra_1="9214717-1";Barra_2="9214717-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00217";TIPO="Não Definida";Circuito_1="PPE-01Y2";Circuito_2="PPE-01Y4";Barra_1="X076061";Barra_2="X076061";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00184";TIPO="Não Definida";Circuito_1="PPE-01Y2";Circuito_2="PRP-01J2";Barra_1="X087475";Barra_2="X087475";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56732";TIPO="Chave a óleo";Circuito_1="PPE-01Y3";Circuito_2="PPE-01Y3";Barra_1="X070570";Barra_2="38616-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56730";TIPO="Não Definida";Circuito_1="PPE-01Y3";Circuito_2="PPE-01Y2";Barra_1="X612207";Barra_2="X612207";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57173";TIPO="Não Definida";Circuito_1="PPE-01Y3";Circuito_2="PPE-01Y6";Barra_1="12865678-1";Barra_2="X615974";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00593";TIPO="Não Definida";Circuito_1="PPE-01Y3";Circuito_2="PRP-01J1";Barra_1="X087498";Barra_2="X087498";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90371";TIPO="Chave Fusível";Circuito_1="PPE-01Y4";Circuito_2="PPE-01Y4";Barra_1="X076517";Barra_2="34830-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57174";TIPO="Não Definida";Circuito_1="PPE-01Y4";Circuito_2="PPE-01Y5";Barra_1="12865677-1";Barra_2="X615973";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00578";TIPO="Não Definida";Circuito_1="PPE-01Y4";Circuito_2="PRP-01J2";Barra_1="9208111-1";Barra_2="9208111-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F38500";TIPO="Não Definida";Circuito_1="PPE-01Y4";Circuito_2="PRP-01J4";Barra_1="X485455";Barra_2="X485455";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00576";TIPO="Não Definida";Circuito_1="PRP-01J2";Circuito_2="PRP-01J4";Barra_1="DX800849";Barra_2="DX800849";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00569";TIPO="Não Definida";Circuito_1="PRP-01J3";Circuito_2="CAJ-01P1";Barra_1="X156999";Barra_2="X156999";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00358";TIPO="Não Definida";Circuito_1="PRP-01J3";Circuito_2="PRP-01J1";Barra_1="9214377-1";Barra_2="9214377-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00188";TIPO="Não Definida";Circuito_1="PRP-01J4";Circuito_2="PRP-01J1";Barra_1="X085266";Barra_2="X085266";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00567";TIPO="Não Definida";Circuito_1="PRP-01J4";Circuito_2="PRP-01J3";Barra_1="9214731-1";Barra_2="9214731-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00287";TIPO="Não Definida";Circuito_1="PRP-01J4";Circuito_2="PPE-01Y4";Barra_1="X081459";Barra_2="X081459";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F36820";TIPO="Seccionalizadora";Circuito_1="PTM-01W1";Circuito_2="PTM-01W1";Barra_1="12026645-1";Barra_2="12026645-1_AUX2";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34667";TIPO="Seccionalizadora";Circuito_1="PTM-01W1";Circuito_2="PTM-01W1";Barra_1="9859638-1";Barra_2="9859638-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51414";TIPO="Não Definida";Circuito_1="PTM-01W1";Circuito_2="PTM-01W4";Barra_1="54466-1";Barra_2="X024120";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50170";TIPO="Não Definida";Circuito_1="PTM-01W2";Circuito_2="PUD-01L3";Barra_1="12484481-1";Barra_2="12484481-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52487";TIPO="Não Definida";Circuito_1="PTM-01W3";Circuito_2="PUD-01L3";Barra_1="X009101";Barra_2="X009101";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00639";TIPO="Não Definida";Circuito_1="PTM-01W3";Circuito_2="SCR-01N8";Barra_1="X011187";Barra_2="X011187";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F53802";TIPO="Chave a óleo";Circuito_1="PTM-01W4";Circuito_2="PTM-01W4";Barra_1="X471441";Barra_2="10198215-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56158";TIPO="Não Definida";Circuito_1="PTM-01W4";Circuito_2="PTM-01W5";Barra_1="X469255";Barra_2="X469255";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00605";TIPO="Não Definida";Circuito_1="PTM-01W5";Circuito_2="SCR-01N4";Barra_1="X065013";Barra_2="X065013";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49857";TIPO="Não Definida";Circuito_1="PTM-01W5";Circuito_2="SCR-01N8";Barra_1="X056632";Barra_2="X056632";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00178";TIPO="Não Definida";Circuito_1="PUD-01L1";Circuito_2="PUD-01P1";Barra_1="X015407";Barra_2="X015407";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00395";TIPO="Não Definida";Circuito_1="PUD-01L1";Circuito_2="PUD-01L2";Barra_1="9751891-1";Barra_2="9751891-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00654";TIPO="Não Definida";Circuito_1="PUD-01L1";Circuito_2="PUD-01L2";Barra_1="12445216-1";Barra_2="12445216-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51999";TIPO="Não Definida";Circuito_1="PUD-01L2";Circuito_2="PUD-01P5";Barra_1="X568110";Barra_2="X568110";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51834";TIPO="Seccionalizadora";Circuito_1="PUD-01L3";Circuito_2="PUD-01L3";Barra_1="X567341";Barra_2="12531326-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00656";TIPO="Não Definida";Circuito_1="PUD-01L3";Circuito_2="PUD-01L2";Barra_1="X005094";Barra_2="X005094";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50055";TIPO="Não Definida";Circuito_1="PUD-01L3";Circuito_2="PUD-01L2";Barra_1="X499483";Barra_2="X499483";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00157";TIPO="Não Definida";Circuito_1="PUD-01L4";Circuito_2="PUD-01P5";Barra_1="X004459";Barra_2="X004459";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24722";TIPO="Não Definida";Circuito_1="PUD-01L4";Circuito_2="PUD-01P5";Barra_1="104071735-1";Barra_2="104071735-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F23894";TIPO="Não Definida";Circuito_1="PUD-01L4";Circuito_2="PTM-01W1";Barra_1="X036420";Barra_2="X036420";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00168";TIPO="Não Definida";Circuito_1="PUD-01L4";Circuito_2="PUD-01L3";Barra_1="X004205";Barra_2="X004205";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00655";TIPO="Não Definida";Circuito_1="PUD-01L4";Circuito_2="PTM-01W1";Barra_1="X027347";Barra_2="X027347";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00420";TIPO="Não Definida";Circuito_1="PUD-01L5";Circuito_2="CJD-01F7";Barra_1="X203544";Barra_2="X203544";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00227";TIPO="Não Definida";Circuito_1="PUD-01L5";Circuito_2="PUD-01L1";Barra_1="X015261";Barra_2="X015261";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00664";TIPO="Não Definida";Circuito_1="PUD-01L5";Circuito_2="PUD-01P1";Barra_1="X020694";Barra_2="X020694";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50226";TIPO="Não Definida";Circuito_1="PUD-01L6";Circuito_2="PUD-01L7";Barra_1="X015873";Barra_2="X015873";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00665";TIPO="Não Definida";Circuito_1="PUD-01L6";Circuito_2="CAB-01C2";Barra_1="X432269";Barra_2="X432269";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00145";TIPO="Não Definida";Circuito_1="PUD-01P1";Circuito_2="CJD-01F6";Barra_1="X033500";Barra_2="X033500";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00661";TIPO="Não Definida";Circuito_1="PUD-01P1";Circuito_2="PUD-01P3";Barra_1="X020128";Barra_2="X020128";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50136";TIPO="Não Definida";Circuito_1="PUD-01P2";Circuito_2="PUD-01L7";Barra_1="12870406-1";Barra_2="X616419";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00187";TIPO="Não Definida";Circuito_1="PUD-01P2";Circuito_2="CAB-01C2";Barra_1="12267103-1";Barra_2="12267103-2";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F32845";TIPO="Não Definida";Circuito_1="PUD-01P2";Circuito_2="CAB-01C4";Barra_1="X048212";Barra_2="X048212";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F34794";TIPO="Não Definida";Circuito_1="PUD-01P2";Circuito_2="PUD-01L6";Barra_1="104867761-2";Barra_2="104867761-2";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00666";TIPO="Não Definida";Circuito_1="PUD-01P2";Circuito_2="CAB-01C1";Barra_1="12325350-1";Barra_2="12325350-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50267";TIPO="Não Definida";Circuito_1="PUD-01P3";Circuito_2="PUD-01L7";Barra_1="12521612-1";Barra_2="12521612-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00440";TIPO="Não Definida";Circuito_1="PUD-01P3";Circuito_2="PUD-01L6";Barra_1="X019519";Barra_2="X019519";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52643";TIPO="Seccionalizadora";Circuito_1="PUD-01P4";Circuito_2="PUD-01P4";Barra_1="12622221-1";Barra_2="12622221-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52003";TIPO="Não Definida";Circuito_1="PUD-01P4";Circuito_2="PUD-01L4";Barra_1="12739767-1";Barra_2="12739767-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52645";TIPO="Seccionalizadora";Circuito_1="PUD-01P5";Circuito_2="PUD-01P5";Barra_1="12622299-1";Barra_2="12622299-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52005";TIPO="Não Definida";Circuito_1="PUD-01P5";Circuito_2="PUD-01P4";Barra_1="95103-1";Barra_2="X004723";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00325";TIPO="Não Definida";Circuito_1="PUD-01P5";Circuito_2="IMB-01J2";Barra_1="X012703";Barra_2="X012703";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00322";TIPO="Não Definida";Circuito_1="PUD-01P5";Circuito_2="IMB-01J3";Barra_1="3493382-1";Barra_2="3493382-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24973";TIPO="Não Definida";Circuito_1="PUD-01P5";Circuito_2="PUD-01L4";Barra_1="104071579-1";Barra_2="104071579-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57172";TIPO="Não Definida";Circuito_1="RET-01F1";Circuito_2="CAB-01S2";Barra_1="X003056";Barra_2="X003056";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00680";TIPO="Não Definida";Circuito_1="RET-01F1";Circuito_2="RET-01Y5";Barra_1="X104186";Barra_2="X104186";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F04049";TIPO="Não Definida";Circuito_1="RET-01F2";Circuito_2="RET-01Y1";Barra_1="X134053";Barra_2="X134053";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49778";TIPO="Não Definida";Circuito_1="RET-01F2";Circuito_2="LPN-01M8";Barra_1="X135197";Barra_2="X135197";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F57272";TIPO="Chave Fusível";Circuito_1="RET-01F3";Circuito_2="RET-01F3";Barra_1="X137759";Barra_2="144226-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00066";TIPO="Não Definida";Circuito_1="RET-01F3";Circuito_2="IPG-01C4";Barra_1="X131843";Barra_2="X131843";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00230";TIPO="Não Definida";Circuito_1="RET-01F3";Circuito_2="RET-01F5";Barra_1="X135004";Barra_2="X135004";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49324";TIPO="Não Definida";Circuito_1="RET-01F4";Circuito_2="RET-01F3";Barra_1="X108140";Barra_2="X108140";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00192";TIPO="Não Definida";Circuito_1="RET-01F4";Circuito_2="CAB-01S2";Barra_1="X003015";Barra_2="X003015";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00060";TIPO="Não Definida";Circuito_1="RET-01F5";Circuito_2="RET-01Y1";Barra_1="X107544";Barra_2="X107544";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00513";TIPO="Não Definida";Circuito_1="RET-01F5";Circuito_2="IPG-01C2";Barra_1="X131684";Barra_2="X131684";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00068";TIPO="Não Definida";Circuito_1="RET-01Y1";Circuito_2="LPN-01M7";Barra_1="X130320";Barra_2="X130320";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00161";TIPO="Não Definida";Circuito_1="RET-01Y3";Circuito_2="MTT-01N1";Barra_1="X106971";Barra_2="X106971";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00234";TIPO="Não Definida";Circuito_1="RET-01Y4";Circuito_2="CAB-01C7";Barra_1="X000988";Barra_2="X000988";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90915";TIPO="Não Definida";Circuito_1="RET-01Y4";Circuito_2="RET-01Y3";Barra_1="X495718";Barra_2="X495718";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00359";TIPO="Não Definida";Circuito_1="RET-01Y4";Circuito_2="MTT-01L5";Barra_1="X105518";Barra_2="X105518";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00115";TIPO="Não Definida";Circuito_1="RET-01Y5";Circuito_2="MTT-01L5";Barra_1="X104374";Barra_2="X104374";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24640";TIPO="Seccionalizadora";Circuito_1="SCR-01J1";Circuito_2="SCR-01J1";Barra_1="X700767";Barra_2="662718-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F24591";TIPO="Não Definida";Circuito_1="SCR-01J1";Circuito_2="IIN-01F2";Barra_1="X501707";Barra_2="X501707";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F29655";TIPO="Seccionalizadora";Circuito_1="SCR-01J2";Circuito_2="SCR-01J2";Barra_1="9859612-1";Barra_2="9859612-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00734";TIPO="Não Definida";Circuito_1="SCR-01J2";Circuito_2="SCR-01J5";Barra_1="X041502";Barra_2="X041502";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00545";TIPO="Não Definida";Circuito_1="SCR-01J2";Circuito_2="SCR-01N5";Barra_1="X041363";Barra_2="X041363";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F51298";TIPO="Não Definida";Circuito_1="SCR-01J2";Circuito_2="SCR-01N6";Barra_1="12526244-1";Barra_2="X705607";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00643";TIPO="Não Definida";Circuito_1="SCR-01J3";Circuito_2="IIN-01F4";Barra_1="X412079";Barra_2="X412079";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F54248";TIPO="Não Definida";Circuito_1="SCR-01J3";Circuito_2="CIT-01Y1";Barra_1="X569337";Barra_2="X569337";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50746";TIPO="Não Definida";Circuito_1="SCR-01J3";Circuito_2="IIN-01F1";Barra_1="X053772";Barra_2="X053772";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49759";TIPO="Não Definida";Circuito_1="SCR-01J3";Circuito_2="SCR-01N9";Barra_1="X045283";Barra_2="X045283";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00718";TIPO="Não Definida";Circuito_1="SCR-01J4";Circuito_2="IIN-01F4";Barra_1="X057174";Barra_2="X057174";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00309";TIPO="Não Definida";Circuito_1="SCR-01J5";Circuito_2="PTM-01W1";Barra_1="X031358";Barra_2="X031358";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00313";TIPO="Não Definida";Circuito_1="SCR-01J5";Circuito_2="SCR-01N3";Barra_1="X034641";Barra_2="X034641";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00590";TIPO="Não Definida";Circuito_1="SCR-01J6";Circuito_2="SCR-01N4";Barra_1="X064653";Barra_2="X064653";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F50242";TIPO="Não Definida";Circuito_1="SCR-01J6";Circuito_2="PTM-01W5";Barra_1="X469248";Barra_2="X469248";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F48432";TIPO="Seccionalizadora";Circuito_1="SCR-01N2";Circuito_2="SCR-01N2";Barra_1="X035061";Barra_2="42484-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00735";TIPO="Não Definida";Circuito_1="SCR-01N2";Circuito_2="SCR-01J5";Barra_1="X039351";Barra_2="X039351";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00736";TIPO="Não Definida";Circuito_1="SCR-01N2";Circuito_2="SCR-01J5";Barra_1="X034771";Barra_2="X034771";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00397";TIPO="Não Definida";Circuito_1="SCR-01N2";Circuito_2="SCR-01N3";Barra_1="X200003";Barra_2="X200003";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F21306";TIPO="Não Definida";Circuito_1="SCR-01N2";Circuito_2="SCR-01N5";Barra_1="X035090";Barra_2="X035090";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00314";TIPO="Não Definida";Circuito_1="SCR-01N3";Circuito_2="SCR-01N5";Barra_1="X036804";Barra_2="X036804";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F29089";TIPO="Seccionalizadora";Circuito_1="SCR-01N5";Circuito_2="SCR-01N5";Barra_1="X501859";Barra_2="827144-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F09457";TIPO="Chave Fusível";Circuito_1="SCR-01N5";Circuito_2="SCR-01N5";Barra_1="104227146-1";Barra_2="104227146-1_AUX1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00315";TIPO="Não Definida";Circuito_1="SCR-01N5";Circuito_2="SCR-01J4";Barra_1="X204087";Barra_2="X204087";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00579";TIPO="Não Definida";Circuito_1="SCR-01N6";Circuito_2="SCR-01J4";Barra_1="104811280-1";Barra_2="104811280-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F90678";TIPO="Não Definida";Circuito_1="SCR-01N6";Circuito_2="SCR-01J3";Barra_1="84513-1";Barra_2="X168589";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F91109";TIPO="Não Definida";Circuito_1="SCR-01N7";Circuito_2="SCR-01N4";Barra_1="X056577";Barra_2="X056577";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00045";TIPO="Não Definida";Circuito_1="SCR-01N7";Circuito_2="SCR-01N9";Barra_1="X501962";Barra_2="X501962";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00636";TIPO="Não Definida";Circuito_1="SCR-01N7";Circuito_2="SCR-01N4";Barra_1="X056575";Barra_2="X056575";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F52739";TIPO="Não Definida";Circuito_1="SCR-01N7";Circuito_2="SCR-01N8";Barra_1="X700214";Barra_2="X700214";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00054";TIPO="Não Definida";Circuito_1="SCR-01N8";Circuito_2="SCR-01N7";Barra_1="X200160";Barra_2="X200160";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F00353";TIPO="Não Definida";Circuito_1="SCR-01N8";Circuito_2="CAJ-01P4";Barra_1="X015080";Barra_2="X015080";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F56272";TIPO="Não Definida";Circuito_1="SCR-01N8";Circuito_2="PUD-01L5";Barra_1="12822974-1";Barra_2="12822974-1";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 Chave="F49757";TIPO="Não Definida";Circuito_1="SCR-01N9";Circuito_2="SCR-01J6";Barra_1="X213207";Barra_2="X213207";if(!InsereChavesNA(Chave, TIPO, Circuito_1, Circuito_2, Barra_1, Barra_2)) rchRes->Lines->Add(Chave);
 */









