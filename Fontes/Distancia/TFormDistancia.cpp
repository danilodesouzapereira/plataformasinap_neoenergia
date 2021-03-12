//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormDistancia.h"
#include "VTCaminho.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"    //DVK 2013.07.22
#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Classes\VTClasse.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Report\VTReport.h"      //DVK 2013.07.22
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Report.h"    //DVK 2013.07.22
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormDistancia::TFormDistancia(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(owner, parent)
   {
   //salva ponteiro p/ objeto
   apl = apl_owner;
   //cria objetos
   lisARR  = new TList();
   lisEQP  = new TList();
   lisLIG  = new TList();
   caminho = NewObjCaminho(apl);
   //inicia estutura de controle de ordenação dos LView
   LViewSort.Column         = NULL;
   LViewSort.OrdemCrescente = true;;
   }

//---------------------------------------------------------------------------
__fastcall TFormDistancia::~TFormDistancia(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
   Moldura(NULL);
   //cancela destaque dos Eqptos
   DestacaEqpto(NULL);
   //destrói objetos
   if (edt_busca) {delete edt_busca; edt_busca = NULL;}
   if (caminho)   {delete caminho;   caminho   = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisARR) {delete lisARR; lisARR = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::ActionEdtExecute(TObject *Sender)
   {
	try{//se necessário, cria um objeto TEdtBusca
      if (edt_busca != NULL) delete edt_busca;
      //cria EdtSpy p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Relatórios_Caminho_Elétrico");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormDistancia::ActionExportaExecute(TObject *Sender)
   {  //DVK 2013.07.22
   //variaveis locais
   AnsiString  filename;
   VTPath      *path   = (VTPath*)apl->GetObject(__classid(VTPath));
   VTReport    *report;

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
   //define pasta inicial
   SaveDialog->InitialDir = path->DirExporta();
   if(SaveDialog->Execute())
      {//exporta conteudo do listView
      filename = SaveDialog->FileName;
      report->ArquivoTexto(filename,LViewResumo, "", false);
      report->ArquivoTexto(filename,LViewEqpto, "", true);
      report->ArquivoTexto(filename,LViewTrecho, "", true);
      report->ArquivoTexto(filename,LViewTrafo, "", true);
      report->ArquivoTexto(filename,LViewChave, "", true);
      if(report->ArquivoTexto(filename,LViewCarga, "", true))
         Aviso("Os dados foram exportados corretamente.");
		else Erro("Erro ao exportar dados.");
      }
   //destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reapresenta form
	Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::ActionSelEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TList   *lisEXT = (TList*)Sender;
   bool flagMontante = CheckBoxMontante->Checked;

   //proteção
   if (lisEXT == NULL) return;
   if (lisEXT->Count == 0) return;
   //determina Eqpto selecionado
   if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL) return;
   //verifica se Eqpto é uma Barra ou Ligacao
   if (eqpto->TipoBarra() || eqpto->TipoLigacao())
      {//define Eqpto inicial ou final
      if      (dist.eqpto_ini == NULL) DefineEqptoInicial(eqpto);
	  else if (dist.eqpto_fim == NULL && flagMontante == false) DefineEqptoFinal(eqpto);
      else                             DefineEqptoInicial(eqpto);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::DefineEqptoFinal(VTEqpto *eqpto)
   {
   //salva Eqpto final
   dist.eqpto_fim = eqpto;
   //determina lista de Eqptos entre Eqpto inicial e final
   lisLIG->Clear();
   caminho->Executa(dist.eqpto_ini, dist.eqpto_fim, lisLIG);
   //reinicia ListView
   ExibeDados(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::DefineEqptoInicial(VTEqpto *eqpto)
   {
   //salva Eqpto inicial
   dist.eqpto_ini = eqpto;
   //cancela Eqpto final
   dist.eqpto_fim = NULL;
   //limpa lista de Eqptos entre Eqpto inicial e final
   lisLIG->Clear();
   // verifica se o checkbox está ativo
   if (CheckBoxMontante->Checked == true)
   {
        // seleciona montante
		SelecionaMontante(eqpto);
   }
   //reinicia ListView
   ExibeDados(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::DestacaEqpto(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->DestacaEqpto(lisEQP, clAqua, 1.5);  //clLime
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::ExibeDados(TList *lisLIG)
   {
   //exibe moldura c/ Eqpto inicial e final
   lisEQP->Clear();
   if (dist.eqpto_ini) lisEQP->Add(dist.eqpto_ini);
   if (dist.eqpto_fim) lisEQP->Add(dist.eqpto_fim);
   Moldura(lisEQP);
   //destaca ligacoes
   DestacaEqpto(lisLIG);
   //seleciona ActivePage
   PageControl->ActivePage = TabSheetResumo;
   //reinicia todos ListView
   LViewCargaInicia(lisLIG);
   LViewEqptoInicia(lisLIG);
   LViewChaveInicia(lisLIG);
   LViewResumoInicia(lisLIG);
   LViewTrafoInicia(lisLIG);
   LViewTrechoInicia(lisLIG);
   LViewArranjoInicia(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtMedidor
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormDistancia::LisArranjo(TList *lisLIG, TList *lisARR)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTTrecho  *trecho;

   //reinicia lisEQP
   lisARR->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {//insere Arranjo em lisARR
         trecho = (VTTrecho*)ligacao;
         if (lisARR->IndexOf(trecho->arranjo) < 0)
            {
            lisARR->Add(trecho->arranjo);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormDistancia::LisBarra(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //reinicia lisEQP
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      ligacao->LisBarra(lisEQP);
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormDistancia::LisEqbar(TList *lisLIG, int tipo_eqpto)
    {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //reinicia lisEQP
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         barra->LisEqbar(lisEQP, tipo_eqpto);
         }
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormDistancia::LisLigacao(TList *lisLIG, int tipo_eqpto)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //reinicia lisEQP
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
	  if (ligacao->Tipo() == tipo_eqpto) lisEQP->Add(ligacao);
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormDistancia::LisRede(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //reinicia lisEQP
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (lisEQP->IndexOf(ligacao->rede) < 0) lisEQP->Add(ligacao->rede);
      }
   return(lisEQP);
   }


//---------------------------------------------------------------------------
TList* __fastcall TFormDistancia::LisTrecho(TList *lisLIG, VTArranjo *arranjo)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTTrecho  *trecho;

   //reinicia lisEQP com Trechos do Arranjo indicado
   lisEQP->Clear();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {
         trecho = (VTTrecho*)ligacao;
         if (trecho->arranjo == arranjo)
            {
            lisEQP->Add(trecho);
            }
         }
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewArranjoInicia(TList *lisLIG)
   {
   //variáveis locais
   AnsiString txt;
   double    comp_m;
   TList     *lisTRE;
   VTArranjo *arranjo;
   VTCabo    *cabo;
   TListView *LView = LViewArranjo;
   TListItem *Item;
   VTFases   *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //reinicia LView
   LView->Clear();
   //obtém lista de Arranjos
   LisArranjo(lisLIG, lisARR);
   //monta lista de Trechos de cada um dos Arranjos
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //monta lista de Trechos do Arranjo
      lisTRE = LisTrecho(lisLIG, arranjo);
      //determina comprimento dos trechos
      comp_m = SomaComprimentoTrecho_m(lisTRE);
      //insere ListItem p/ cada Arranjo
      Item          = LView->Items->Add();
      Item->Data    = arranjo;
      Item->Caption = arranjo->Codigo;
      Item->SubItems->Add(DoubleToStr("%2.1f", arranjo->Iadm_amp));
      Item->SubItems->Add(DoubleToStr("%2.1f", comp_m));
      if (arranjo->TipoCfg == arranjoCABOxFASE)
         {//exibe Cabos das fases
         txt = "";
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            if ((cabo = arranjo->fase[ind_fase].cabo) == NULL) continue;
            txt += fases->AsStrABCN[fases->IndexToTag[ind_fase]] + "(" + cabo->Codigo + ") ";
            }
         }
      else if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {//exibe CaboZ0Z1
         txt = arranjo->caboZ0Z1->Codigo;
         }
      else
         {//não usa definição por cabo
         txt = "não especificado";
         }
      Item->SubItems->Add(txt);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewCargaInicia(TList *lisLIG)
   {
   //variáveis locais
   VTCarga   *carga;
   VTEqpto   *eqpto;
   VTRede    *rede;
   TListView *LView = LViewCarga;
   TListItem *Item;

   //reinicia LView
   LView->Clear();
   //obtém lista de Cargas
   lisEQP = LisEqbar(lisLIG, eqptoCARGA);
   if (lisEQP->Count == 0) return;
   //insere ListItem p/ cada Chave
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      //determina uma Rede que contém a Barra da Carga
      rede = RedeDaBarra(lisLIG, carga->pbarra);
      Item = LView->Items->Add();
      Item->Data    = carga;
      Item->Caption = (rede != NULL) ? rede->Codigo : AnsiString("isolada");
      Item->SubItems->Add(carga->Codigo);
      Item->SubItems->Add(carga->classe->Codigo);
      if (carga->curva->Tipica)
         {Item->SubItems->Add(DoubleToStr("%2.1f", carga->Energia_mwhmes * 1000.));}
      else
         {Item->SubItems->Add("curva de demanda");}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewChaveInicia(TList *lisLIG)
   {
   //variáveis locais
   VTChave *chave;
   VTEqpto *eqpto;
   TListView *LView = LViewChave;
   TListItem *Item;

   //reinicia LView
   LView->Clear();
   //insere ListItem p/ cada Chave
   for (int n = 0; n < lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[n];
      if (eqpto->Tipo() != eqptoCHAVE) continue;
      chave = (VTChave*)eqpto;
      Item = LView->Items->Add();
      Item->Data   = chave;
      Item->Caption = chave->rede->Codigo;
      Item->SubItems->Add(chave->TipoChave->Codigo);
      Item->SubItems->Add(chave->Codigo);
      Item->SubItems->Add(DoubleToStr("%2.1f", chave->Inom_amp));
      Item->SubItems->Add((chave->Aberta) ? "aberta" : "fechada");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewClick(TObject *Sender)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TListView *LView = (TListView*)Sender;

   //verifica se há um ListItem selecionado
   if (LView->Selected == NULL) return;
   if ((eqpto = (VTEqpto*)LView->Selected->Data) == NULL) return;
   //destaca Eqpto c/ Moldura
   lisEQP->Clear();
   if (dist.eqpto_ini) lisEQP->Add(dist.eqpto_ini);
   if (dist.eqpto_fim) lisEQP->Add(dist.eqpto_fim);
   lisEQP->Add(eqpto);
   Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.Column == Column)
      {//inverte ordem
      LViewSort.OrdemCrescente  = ! LViewSort.OrdemCrescente;
      }
   else
      {//salva coluna usada p/ ordenação
      LViewSort.Column         = Column;
      LViewSort.OrdemCrescente = true;
      }
   //força reordenação
   ((TListView*)Sender)->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewCompare(TObject *Sender,
                TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int       index;
   double    valor1, valor2;

   //proteção
   if (LViewSort.Column == NULL) return;
   //determina index p/ substrings
   index = LViewSort.Column->Index - 1;
   //verifica se coluna de Caption
   if (LViewSort.Column->Index == 0)
      {//coluna de Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   else if (LViewSort.Column->Alignment == taLeftJustify)
      {//coluna com texto
      Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
      }
   else
      {//coluna com valor numérico
      valor1  = StrToDouble(Item1->SubItems->Strings[index]);
      valor2  = StrToDouble(Item2->SubItems->Strings[index]);
      if      (valor1 < valor2) Compare = -1;
      else if (valor1 > valor2) Compare =  1;
      else                      Compare =  0;
      }
   //verifica se ordem crescente/decrescente
   if (! LViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewDblClick(TObject *Sender)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TListView *LView = (TListView*)Sender;

   //verifica se há um ListItem selecionado
   if (LView->Selected == NULL) return;
   if ((eqpto = (VTEqpto*)LView->Selected->Data) == NULL) return;
   //destaca Eqpto c/ Zoom
   Zoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewEqptoInicia(TList *lisLIG)
   {
   //variáveis locais
   VTChave   *chave;
   VTLigacao *ligacao;
   VTTrafo   *trafo;
   VTTrafo3E *trafo3E;
   VTTrecho  *trecho;
   TListView *LView = LViewEqpto;
   TListItem *Item;

   //reinicia LView
   LView->Clear();
   //insere ListItem p/ cada Chave
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      Item = LView->Items->Add();
      Item->Data    = ligacao;
      Item->Caption = ligacao->rede->Codigo;
      Item->SubItems->Add(ligacao->TipoAsString());
      Item->SubItems->Add(ligacao->Codigo);
      switch(ligacao->Tipo())
         {
         case eqptoCHAVE:
            chave = (VTChave*)ligacao;
            Item->SubItems->Add(chave->TipoChave->Codigo);
            break;
         case eqptoTRAFO:
            trafo = (VTTrafo*)ligacao;
            Item->SubItems->Add(DoubleToStr("%4.3f MVA", trafo->snom));
            break;
         case eqptoTRAFO3E:
            trafo3E = (VTTrafo3E*)ligacao;
            Item->SubItems->Add(DoubleToStr("%4.3f MVA", trafo3E->Sbase));
            break;
         case eqptoTRECHO:
            trecho = (VTTrecho*)ligacao;
            Item->SubItems->Add(trecho->arranjo->Codigo);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewResumoInicia(TList *lisLIG)
   {
   //variáveis locais
   TList     *lisEQP;
   TListItem *Item;
   TListView *LView = LViewResumo;
   int        vet_ligacao[] = {eqptoTRECHO,    eqptoTRAFO,     eqptoTRAFO3E,
                               eqptoREGULADOR, eqptoCAPSERIE,  eqptoCHAVE};
   int        vet_eqbar[]   = {eqptoCARGA,    eqptoCAPACITOR, eqptoGERADOR,
                               eqptoSUPRIMENTO};

   //reinicia LView
   LView->Clear();
   //proteção
   if (lisLIG->Count == 0) return;
   //exibe número de redes
   lisEQP = LisRede(lisLIG);
   Item = LView->Items->Add();
   Item->Caption = "Redes";
   Item->SubItems->Add(IntToStr(lisEQP->Count));
   //exibe número de barras
   lisEQP = LisBarra(lisLIG);
   Item = LView->Items->Add();
   Item->Caption = "Barras";
   Item->SubItems->Add(IntToStr(lisEQP->Count));
   //exibe número de Ligacoes
   for (int n = 0; n < sizeof(vet_ligacao)/sizeof(int); n++)
      {//obtém lista de Eqptos do tipo
      lisEQP = LisLigacao(lisLIG, vet_ligacao[n]);
      if (lisEQP->Count == 0) continue;
      Item = LView->Items->Add();
      Item->Caption = VTEqpto::TipoEqpto_AsString(vet_ligacao[n]);
      Item->SubItems->Add(IntToStr(lisEQP->Count));
      if (vet_ligacao[n] == eqptoTRECHO)
         {//exibe comprimento total dos trechos
         Item->SubItems->Add(DoubleToStr("%2.1f", SomaComprimentoTrecho_m(lisEQP)));
         }
      }
   //exibe número de Eqbar
   for (int n = 0; n < sizeof(vet_eqbar)/sizeof(int); n++)
      {//obtém lista de Eqptos do tipo
      lisEQP = LisEqbar(lisLIG, vet_eqbar[n]);
      if (lisEQP->Count == 0) continue;
      Item = LView->Items->Add();
      Item->Caption = VTEqpto::TipoEqpto_AsString(vet_eqbar[n]);
      Item->SubItems->Add(IntToStr(lisEQP->Count));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewTrafoInicia(TList *lisLIG)
   {
   //variáveis locais
   AnsiString tipo_ligacao;
   TList     *lisTRF;
   VTTrafo   *trafo;
   TListView *LView = LViewTrafo;
   TListItem *Item;

   //reinicia LView
   LView->Clear();
   //obtém lista de Trafos
   lisTRF = LisLigacao(lisLIG, eqptoTRAFO);
   //insere ListItem p/ cada Trafo
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      Item = LView->Items->Add();
      Item->Data   = trafo;
      Item->Caption = trafo->rede->Codigo;
      Item->SubItems->Add(trafo->Codigo);
      Item->SubItems->Add(DoubleToStr("%2.1f", trafo->snom * 1000.));
      //define tipo de ligação
      if (trafo->Monofasico)
         {tipo_ligacao = "monofásico";}
      else
         {//determina tipo da ligação do primário/secundário
         switch(trafo->pri.ligacao)
            {
            case lgEST_AT:  tipo_ligacao = "Yat"; break;
            case lgTRIANG:  tipo_ligacao = "D";   break;
            case lgEST_ISO: tipo_ligacao = "Y";   break;
            default:        tipo_ligacao = "-";   break;
            }
         switch(trafo->sec.ligacao)
            {
            case lgEST_AT:  tipo_ligacao += "|Yat"; break;
            case lgTRIANG:  tipo_ligacao += "|D";   break;
            case lgEST_ISO: tipo_ligacao += "|Y";   break;
            default:        tipo_ligacao += "|-";   break;
            }
         }
      Item->SubItems->Add(tipo_ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::LViewTrechoInicia(TList *lisLIG)
   {
   //variáveis locais
   AnsiString txt;
   TList     *lisTRE;
   VTCabo    *cabo;
   VTEqpto   *eqpto;
   VTTrecho  *trecho;
   TListView *LView = LViewTrecho;
   TListItem *Item;
   VTFases   *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //reinicia LView
   LView->Clear();
   //obtém lista de Trechos
   lisTRE = LisLigacao(lisLIG, eqptoTRECHO);
   //insere ListItem p/ cada Chave
   for (int n = 0; n < lisTRE->Count; n++)
      {
      eqpto = (VTEqpto*)lisTRE->Items[n];
      if (eqpto->Tipo() != eqptoTRECHO) continue;
      trecho = (VTTrecho*)eqpto;
      Item = LView->Items->Add();
      Item->Data   = trecho;
      Item->Caption = trecho->rede->Codigo;
      Item->SubItems->Add(trecho->Codigo);
      Item->SubItems->Add(DoubleToStr("%2.1f", trecho->Comprimento_m));
      Item->SubItems->Add(trecho->arranjo->Codigo);
      if (trecho->arranjo->TipoCfg == arranjoCABOxFASE)
         {//exibe Cabos das fases
         txt = "";
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            if ((cabo = trecho->arranjo->fase[ind_fase].cabo) == NULL) continue;
            txt += fases->AsStrABCN[fases->IndexToTag[ind_fase]] + "(" + cabo->Codigo + ") ";
            }
         }
      else if (trecho->arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {//exibe CaboZ0Z1
         txt = trecho->arranjo->caboZ0Z1->Codigo;
         }
      else
         {//não usa definição por cabo
         txt = "não especificado";
         }
      Item->SubItems->Add(txt);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::Moldura(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::SelecionaMontante(VTEqpto *eqpto)
   {
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList *lisEQPTOinicial = new TList();
   TList *lisLigacoes = new TList();
   TList *lisSuprimentos = new TList();

   //preenche as listas criadas para executar o caminho
   lisEQPTOinicial->Add(eqpto);
   redes->LisLigacao(lisLigacoes);
   redes->LisEqpto(lisSuprimentos, eqptoSUPRIMENTO);

   //limpa a lista de ligações e executa o caminho
   lisLIG->Clear();
   caminho->Executa(lisEQPTOinicial, lisSuprimentos, lisLigacoes, lisLIG);

   //reinicia ListView
   ExibeDados(lisLIG);

   //deleta as listas criadas
   if (lisEQPTOinicial) {delete lisEQPTOinicial; lisEQPTOinicial = NULL;}
   if (lisLigacoes) {delete lisLigacoes; lisLigacoes = NULL;}
   if (lisSuprimentos) {delete lisSuprimentos; lisSuprimentos = NULL;}

   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormDistancia::RedeDaBarra(TList *lisLIG, VTBarra *barra)
   {
   //veriáveis locais
   VTLigacao *ligacao;

   //reinicia lisEQP
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->ConectaBarra(barra)) return(ligacao->rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
double __fastcall TFormDistancia::SomaComprimentoTrecho_m(TList *lisTRE)
   {
   //variáveis locais
   double   comp_m = 0;
   VTTrecho *trecho;

   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      comp_m += trecho->Comprimento_m;
      }
   return(comp_m);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDistancia::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof


