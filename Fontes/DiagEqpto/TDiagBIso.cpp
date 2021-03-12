//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
//#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagBIso.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall  DIAG_ISOLADO::DIAG_ISOLADO(int id, AnsiString codigo)
{
   this->id = id;
   this->codigo = codigo;
   finalizado = false;
   lisBarras = new TList;
   lisLigacoes = new TList;
   lisEqpto = new TList;
   this->isolado = true;
}
//---------------------------------------------------------------------------
__fastcall  DIAG_ISOLADO::~DIAG_ISOLADO(void)
{
   //destroi lista
   if(lisBarras){delete lisBarras; lisBarras = NULL;}
   //destroi lista
   if(lisLigacoes){delete lisLigacoes; lisLigacoes = NULL;}
   //destroi lista
   if(lisEqpto){delete lisEqpto; lisEqpto = NULL;}
}//---------------------------------------------------------------------------TList*  __fastcall  DIAG_ISOLADO::ConcatenaLisEqpto(void)
{  //limpa a lista
   lisEqpto->Clear();
   //adiciona barras e ligacoes à lista de eqptos
   for(int nb = 0; nb <lisBarras->Count; nb++)
   {
      lisEqpto->Add(lisBarras->Items[nb]);
   }
   for(int nl = 0; nl <lisLigacoes->Count; nl++)
   {
      lisEqpto->Add(lisLigacoes->Items[nl]);
   }
   return lisEqpto;
}
//---------------------------------------------------------------------------
//void  __fastcall  DIAG_ISOLADO::Area(void)
//{  //variaveis locais
//   //utm
//   double   maiorXutm, maiorYutm;
//   double   menorXutm, menorYutm;
//   //esq
//   double   maiorXesq, maiorYesq;
//   double   menorXesq, menorYesq;
//   VTBarra  *barra;
//
//   for(int nb = 0; nb < lisBarras->Count; nb++)
//   {
//      barra = (VTBarra*)lisBarras->Items[nb];
//      if(nb == 0)
//      {  //pra 1a iteracao
//         maiorXutm = menorXutm = barra->utm.x;
//         maiorYutm = menorYutm = barra->utm.y;
//         if(barra->esq.x1 > barra->esq.x2) {maiorXesq = barra->esq.x1;}
//         else {maiorXesq = barra->esq.x2;}
//         if(barra->esq.y1 > barra->esq.y2) {maiorYesq = barra->esq.y1;}
//         else {maiorYesq = barra->esq.y2;}
//      }
//      else
//      {  //guarda os maiores e menores
//         if(barra->utm.x > maiorXutm) maiorXutm = barra->utm.x;
//         if(barra->utm.x < menorXutm) menorXutm = barra->utm.x;
//         if(barra->utm.y > maiorYutm) maiorYutm = barra->utm.y;
//         if(barra->utm.y < menorYutm) menorYutm = barra->utm.y;
//         //maiores X e Y esq
//         if(barra->esq.x1 > maiorXesq) maiorXesq = barra->esq.x1;
//         if(barra->esq.x2 > maiorXesq) maiorXesq = barra->esq.x2;
//         if(barra->esq.y1 > maiorYesq) maiorYesq = barra->esq.y1;
//         if(barra->esq.y2 > maiorYesq) maiorYesq = barra->esq.y2;
//         //menores X e Y esq
//         if(barra->esq.x1 < menorXesq) menorXesq = barra->esq.x1;
//         if(barra->esq.x2 < menorXesq) menorXesq = barra->esq.x2;
//         if(barra->esq.y1 < menorYesq) menorYesq = barra->esq.y1;
//         if(barra->esq.y2 < menorYesq) menorYesq = barra->esq.y2;
//      }
//   }
//   //monta a area utm
//   area_utm->x1 = menorXutm;
//   area_utm->y1 = menorYutm;
//   area_utm->x2 = maiorXutm;
//   area_utm->y2 = maiorYutm;
//   //monta a area esquematico
//   area_esq->x1 = menorXesq;
//   area_esq->y1 = menorYesq;
//   area_esq->x2 = maiorXesq;
//   area_esq->y2 = maiorYesq;
//}
//---------------------------------------------------------------------------
void  __fastcall  DIAG_ISOLADO::Finaliza(void)
{
//   this->Area();
   this->ConcatenaLisEqpto();
   finalizado = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall  TDiagBIso::TDiagBIso(VTApl *apl)
{  //salva o ponteiro do apl
   this->apl = apl;
   //Mostra que ainda não foi executado
   executado = false;
   //define se tem informações adicionais
   infoExtra = false;
   //cria as listas
   listCfg = new TList;
   listaBlocosIsolados = new TList;
   lisBarrasIsoladas = new TList();
   //inicializa as opcoes do diagnostico
   dgBlocoIsolado.eletric = true;
   //cria o log_barra
   log_biso = new TStringList;
}
//---------------------------------------------------------------------------
__fastcall  TDiagBIso::~TDiagBIso(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi lista e conteudo
   if(listaBlocosIsolados){ LimpaTList(listaBlocosIsolados); delete listaBlocosIsolados; listaBlocosIsolados = NULL;}
   //destroi lista
   if(lisBarrasIsoladas){delete lisBarrasIsoladas; lisBarrasIsoladas = NULL;}
   //destroi log
   if(log_biso){delete log_biso; log_biso = NULL;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBIso::AddLinhaLog( DIAG_ISOLADO *blocoIso, int opDIAG_ISOLADO)
{  //incrementa o contador de erros
   //infoBlocoIsolados.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_TRECHO
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colBIso_COUNT];
   AnsiString  Separador = ";";

   if(blocoIso)
   {
      linha[colBIso_CODIGO]    = blocoIso->codigo + Separador;
   }
   else
   {
      linha[colBIso_CODIGO]    = "-" + Separador;

   }
   if (blocoIso)
   {
      switch (opDIAG_ISOLADO)
      {
         case opBIsoELETRIC:
            linha[colBIso_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colBIso_DESCRICAO] = "Bloco isolado eletricamente.";
            break;

         case opBIsoDEBUG:
            linha[colBIso_TIPOERRO]  = "(DEBUG)" + Separador;
            linha[colBIso_DESCRICAO] = "barra inicio bloco";
            break;

         default:
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colBIso_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_biso->AddObject(linhaPreenchida,blocoIso);
   }
}

////---------------------------------------------------------------------------
//bool  __fastcall  TDiagBIso::Executa(void)
//{  //variaveis locais
//   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
//   VTZonas     *zonas =  (VTZonas*) apl->GetObject(__classid(VTZonas));
//   VTBarra     *barra;
//   TList       *lisBarra;
//
//   //inicializa o Zonas
//   zonas->ChaveAbertaEnabled = false;
//   zonas->Executa();
//   //inicialza infos
//   infoBlocoIsolados.n_total = 1;
//   infoBlocoIsolados.n_verif = 1;
//   infoBlocoIsolados.n_ocorrencias = 0;
//   if(log_biso)IniciaLog(log_biso);
//
////   //inicializa o vetor contador de eqptos isolados
////   for(int i = 0; i < TIPOSEQPTO_COUNT; i++)
////   {
////      tipoEqptoIsolado[i] = 0;
////   }
//   //limpa a lista de barras isoladas
//   lisBarrasIsoladas->Clear();
//   try
//   {  //percorre todas as barras
//      lisBarra = redes->LisBarra();
//      for(int nb = 0; nb < lisBarra->Count; nb++)
//      {
//         barra = (VTBarra*) lisBarra->Items[nb];
//         //monta uma lista de barras sem zona
//         if(barra->zona == NULL)
//         {
//            lisBarrasIsoladas->Add(barra);
//         }
//      }
//      //lista os blocos (lista de listas de barras)
//      ListaBlocos();
//      //conta quantos blocos isolados existem
//      infoBlocoIsolados.n_total = this->listaBlocosIsolados->Count;
//
//      //altera o boolean mostrando que foi executado
//      executado = true;
//      return true;
//   }
//   catch(Exception &e)
//   {return false;}
//}
//---------------------------------------------------------------------------
bool  __fastcall  TDiagBIso::Executa(void)
{  //variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   //VTZonas     *zonas =  (VTZonas*) apl->GetObject(__classid(VTZonas));
   VTBarra     *barra;
   TList       *lisBarra;
   DIAG_ISOLADO *blocoIso;
   bool        sucesso = true;
   VTEqbar     *eqbar;

   //inicialza infos
   infoBlocoIsolados.n_total = 1;
   infoBlocoIsolados.n_verif = 1;
   infoBlocoIsolados.n_ocorrencias = 0;
   if(log_biso)IniciaLog(log_biso);

   //limpa a lista de barras isoladas
   lisBarrasIsoladas->Clear();
   try
   {  //percorre todas as barras
      lisBarra = redes->LisBarra();
	  //preenche a lista com todas as barras da rede
	  lisBarrasIsoladas->Assign(lisBarra,laCopy);
	  //prepara as tags das barras
	  for (int nba = 0; nba < lisBarrasIsoladas->Count; nba++)
	  {
		barra = (VTBarra*)lisBarrasIsoladas->Items[nba];
        barra->Tag = 0;
	  }
	  //marca as barras com suprimentos e geradores com tag 1
	  TagBarrasComGeracao();
      //lista os blocos (lista de listas de barras)
	  ListaBlocos();
	  //percorre todos os blocos, se não for isolado, descarta
	  for(int nb = (this->listaBlocosIsolados->Count - 1);  nb >= 0; nb--)
	  {
		 blocoIso = (DIAG_ISOLADO*)this->listaBlocosIsolados->Items[nb];
		 if (!blocoIso->isolado)
		 {
			this->listaBlocosIsolados->Delete(nb);
		 }
	  }
      infoBlocoIsolados.n_total = this->listaBlocosIsolados->Count;
      //Percorrre a lista de blocos, verificando se vale a pena listar, e contabiliza
	  for(int nb = 0;  nb < this->listaBlocosIsolados->Count; nb++)
      {
         //FKM 2016.02.15 - Para a Dani haha, não listar bloco que é uma barra ligada a chave isolada
		 blocoIso = (DIAG_ISOLADO*)this->listaBlocosIsolados->Items[nb];
         if(VerificaRelevanciaBloco(blocoIso))
         {
            AddLinhaLog(blocoIso,opBIsoELETRIC);
            //incrementa o contador de ocorencias
            infoBlocoIsolados.n_ocorrencias++;
         }
          //FKM 2016.02.15 - Para a Dani haha
      }
      //altera o boolean mostrando que foi executado
      executado = true;
   }
   catch(Exception &e)
   {sucesso = false;}

   return sucesso;
}

//---------------------------------------------------------------------------
//bool   __fastcall  TDiagBIso::ExisteCfgTipoRede(int redeSEGMENTO)
//{  //variaveis locais
//   DIAG_TRECHO *diagTrecho;
//
//   for(int nt = 0; nt < LisCfgDiagTrecho->Count; nt++)
//   {
//      diagTrecho = (DIAG_TRECHO*) LisCfgDiagTrecho->Items[nt];
//      if (diagTrecho->tipo_rede == redeSEGMENTO) {return true;}
//   }
//   return false;
//}

//---------------------------------------------------------------------------
//DIAG_TRECHO* __fastcall  TDiagBIso::Get_DiagTrecho(int redeSEGMENTO)
//{  //variaveis locais
//   DIAG_TRECHO *diagTrecho;
//
//   for(int i = 0 ; i < LisCfgDiagTrecho->Count; i++)
//   {
//      diagTrecho = (DIAG_TRECHO*) LisCfgDiagTrecho->Items[i];
//      if(redeSEGMENTO == diagTrecho->tipo_rede){return diagTrecho;}
//   }
//   return NULL;
//}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBIso::Inicia(void)
{
//   //primeiramente carrega as configs do arquivo
//   LeArquivo();
//   //verifica se falta configs pros tipos de rede existentes
//   VerificaTiposRede();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBIso::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();
   //checkListBox->Items->Add("Coordenadas nulas");
   checkListBox->Items->Add(op_BISO_TEXT[opBIsoELETRIC]);
   checkListBox->Checked[opBIsoELETRIC] = this->OpcaoDiagEletric;
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagBIso::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colBIso_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {
      //limpa o log
      log->Clear();

//      linha[colBIso_SEGMENTO] = "Segmento" + Separador;
//      linha[colBIso_REDE] = "Rede" + Separador;
//      linha[colBIso_EQPTO] = "Eqpto" + Separador;
      linha[colBIso_CODIGO] = "Código" + Separador;
      linha[colBIso_TIPOERRO] = "Tipo Erro" + Separador;
//      linha[colBIso_UTMX] = "UTM X (m)" + Separador;
//      linha[colBIso_UTMY] = "UTM Y (m)" + Separador;
      linha[colBIso_DESCRICAO] = "Descrição";
      //concatena os campos em uma linha
      for (int i = 0; i < colBIso_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
long __fastcall TDiagBIso::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoBlocoIsolados.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagBIso::PM_GetLog(void)
{
   if(executado)
   {
      return log_biso;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagBIso::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoBlocoIsolados.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}
//---------------------------------------------------------------------------
void     __fastcall  TDiagBIso::PreencheListView(TListView *listView)
{  //variaveis locais
   TListItem   *listItem;
   TListColumn *listColumn;
   TStringList *campos = new TStringList();

   //tentar melhorar performance do listview
   listView->Items->BeginUpdate();
   //limpa o listView
   listView->Clear();
   listView->Columns->Clear();

   campos->Clear();
   //Extrai campos da linha
   ExtraiStrings(log_biso->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colBIso_CODIGO];

   listColumn = listView->Columns->Add();
   listColumn->Width = 60;
   listColumn->Caption = campos->Strings[colBIso_TIPOERRO];

   listColumn = listView->Columns->Add();
   listColumn->Width = 150;
   listColumn->Caption = campos->Strings[colBIso_DESCRICAO];

   for(int nl = 1; nl < log_biso->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_biso->Strings[nl], ";", campos);
      listItem = listView->Items->Add();
      listItem->Data = log_biso->Objects[nl];
      listItem->Caption = campos->Strings[0] ;
      for(int nc = 1; nc < campos->Count; nc++)
      {
         listItem->SubItems->Add(campos->Strings[nc]);
      }
   }
   //destroi o TStrings
   if(campos){delete campos; campos = NULL;}
   //tentar melhorar performance do listview
   listView->Items->EndUpdate();
}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagBIso::LeArquivo(void)
{  //variaveis locais
//   VTInfoset   *infoset;
//   DIAG_TRECHO *diagTrecho;
//   VTTipos     *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
//   int         tipoRede;
//   bool        temPadrao = false;
//
//   try {
//      //cria novo infoset para ler o arquivo de cfg
//      infoset = geral->Infoset;
//      infoset->Clear();
//      //verifica se é possivel ler o arquivo
//      if(infoset->LoadFromFile(FILE_CFG_TRECHO))
//      {  //varre todo o arquivo lido
//         for (int i = 0; i < infoset->Count; i++)
//         {
//            infoset->SelectLine(i);
//            //verifica qual é o tipo rede  (redeSEGMENTO)
//            infoset->GetInfo("tipo_rede", tipoRede);
//            ///verifica se o tipo a ser carregado do arquivo existe na base
//            //ou se é o tipo "default"
//            if ((tipos->ExisteTipoRede(tipoRede)) || (tipoRede == 0))
//            {  //caso exista, cria-se um novo objeto
//               diagTrecho = new DIAG_TRECHO;
//               infoset->GetInfo("tipo_rede", diagTrecho->tipo_rede);
//               infoset->GetInfo("nome_tipo", diagTrecho->nome_tipo);
//               infoset->GetInfo("comp_min_m", diagTrecho->comp_min_m);
//               infoset->GetInfo("comp_max_m", diagTrecho->comp_max_m);
//               infoset->GetInfo("iadm_min_a", diagTrecho->iadm_min_a);
//               infoset->GetInfo("iadm_max_a", diagTrecho->iadm_max_a);
//               //infoset->AddInfo("pu", diagTrecho->pu);
//               infoset->GetInfo("seq0_r_max_ohmkm", diagTrecho->seq0_r_max_ohmkm);
//               infoset->GetInfo("seq0_r_min_ohmkm", diagTrecho->seq0_r_min_ohmkm);
//               infoset->GetInfo("seq0_x_max_ohmkm", diagTrecho->seq0_x_max_ohmkm);
//               infoset->GetInfo("seq0_x_min_ohmkm", diagTrecho->seq0_x_min_ohmkm);
//               infoset->GetInfo("seq0_c_max_nFkm", diagTrecho->seq0_c_max_nFkm);
//               infoset->GetInfo("seq0_c_min_nFkm", diagTrecho->seq0_c_min_nFkm);
//               infoset->GetInfo("seq1_r_max_ohmkm", diagTrecho->seq1_r_max_ohmkm);
//               infoset->GetInfo("seq1_r_min_ohmkm", diagTrecho->seq1_r_min_ohmkm);
//               infoset->GetInfo("seq1_x_max_ohmkm", diagTrecho->seq1_x_max_ohmkm);
//               infoset->GetInfo("seq1_x_min_ohmkm", diagTrecho->seq1_x_min_ohmkm);
//               infoset->GetInfo("seq1_c_max_nFkm", diagTrecho->seq1_c_max_nFkm);
//               infoset->GetInfo("seq1_c_min_nFkm", diagTrecho->seq1_c_min_nFkm);
//               LisCfgDiagTrecho->Add(diagTrecho);
//               //caso o tipo rede seja 0 (== PADRAO)
//               if(tipoRede == 0){temPadrao = true;}
//            }
//         }
//      }//caso não exista o arquivo
//      else
//      {  //cria configs para todos os
//         VerificaTiposRede();
//         //cria o tipo "default"
//         diagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
//         //define como padrao
//         //diagTrechoPADRAO = diagTrecho;
//         //adiciona à lista de configs
//         LisCfgDiagTrecho->Add(diagTrecho);
//         temPadrao = true;
//         //e cria o arquivo
//         SalvaArquivo();
//      }
//      if (!temPadrao)
//      {  //cria o tipo "default"
//         diagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
//         //define como padrao
//         //diagTrechoPADRAO = diagTrecho;
//         //adiciona à lista de configs
//         LisCfgDiagTrecho->Add(diagTrecho);
//         temPadrao = true;
//      }
//   return true;
//   } catch (Exception &e) {return false;}
   return false;
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBIso::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagEletric   = checkListBox->Checked[opBIsoELETRIC];
}
//---------------------------------------------------------------------------
void  __fastcall TDiagBIso::ListaBarrasLigadas(VTBarra *barra,  DIAG_ISOLADO *blocoIso)
{  //variaveis locais
   VTBarra     *barra1, *barra2, *barra3;
   VTLigacao   *ligacao;
   TList       *lisLigTemp = new TList;
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));

   //preenche a lista com as ligações
   redes->LisLigacao(lisLigTemp, barra);

   //percorre a lista para listar as barras conectadas a essa barra
   for(int nl = 0; nl < lisLigTemp->Count; nl++)
   {
      ligacao = (VTLigacao*) lisLigTemp->Items[nl];
      //garante a unicidade de cada ligacao na lista do bloco (custa desempenho)
      if(blocoIso->lisLigacoes->IndexOf(ligacao) < 0)blocoIso->lisLigacoes->Add(ligacao);
	  barra1 = ligacao->pbarra1;
	  barra2 = ligacao->pbarra2;
	  barra3 = ligacao->pbarra3;
      //se a barra existir e for diferente da analizada
	  if((barra1) && (barra1 != barra))
      {  //garante a unicidade de cada barra na lista do bloco (custa desempenho)
		 if(blocoIso->lisBarras->IndexOf(barra1) < 0)blocoIso->lisBarras->Add(barra1);
		 if (barra1->Tag == 1)
			{blocoIso->isolado = false;}
	  }
	  if((barra2) && (barra2 != barra))
      {  //garante a unicidade de cada barra na lista do bloco (custa desempenho)
		 if(blocoIso->lisBarras->IndexOf(barra2) < 0)blocoIso->lisBarras->Add(barra2);
		 if (barra2->Tag == 1)
			{blocoIso->isolado = false;}
	  }
      if((barra3) && (barra3 != barra))
      {  //garante a unicidade de cada barra na lista do bloco (custa desempenho)
		 if(blocoIso->lisBarras->IndexOf(barra3) < 0)blocoIso->lisBarras->Add(barra3);
		 if (barra3->Tag == 1)
			{blocoIso->isolado = false;}
      }
   }
   //deleta a lista
   if(lisLigTemp){delete lisLigTemp; lisLigTemp = NULL;}
}
//---------------------------------------------------------------------------
//TList*   __fastcall  TDiagBIso::ListaBlocos(void)
//{  //variaveis locais
//   VTBarra        *barra, *barraBloco;
//   DIAG_ISOLADO   *blocoIso;
//   int            numBloco = 1;
//
//   //protecao
//   if(!listaBlocosIsolados)return NULL;
//   if(!lisBarrasIsoladas)return NULL;
//   //limpa a lista de blocos isolados, mas destruindo a listas dentro (espero!)
//   else{LimpaTList(listaBlocosIsolados);}
//   //percorre todas as barras isoladas disponibilizadas
//   while (lisBarrasIsoladas->Count > 0)
//   {  //cria uma lista de barras isoladas que serão um bloco
//      blocoIso = new DIAG_ISOLADO(numBloco,"Bloco " +IntToStr(numBloco));
//      //incrementa o contador de ocorencias
//      infoBlocoIsolados.n_ocorrencias++;
//      //adiciona o bloco a lista de blocos
//      listaBlocosIsolados->Add(blocoIso);
//      //analisa uma barra
//      barra = (VTBarra*) lisBarrasIsoladas->First();
//      AddLinhaLog(blocoIso,opBIsoELETRIC);
//      //adiciona a barra "inicial" a lista
//      blocoIso->lisBarras->Add(barra);
//      for(int i = 0; i < blocoIso->lisBarras->Count; i++)
//      {
//         barraBloco = (VTBarra*) blocoIso->lisBarras->Items[i];
//         ListaBarrasLigadas(barraBloco,blocoIso);
//         //remove barra da lista de barras isoladas
//         lisBarrasIsoladas->RemoveItem(barraBloco,FromBeginning);
//      }
//      //finaliza a edição do bloco isolado
//      blocoIso->Finaliza();
//      numBloco++;
//   }
//   return listaBlocosIsolados;
//   //delete
//}
//---------------------------------------------------------------------------
TList*   __fastcall  TDiagBIso::ListaBlocos(void)
{  //variaveis locais
   VTBarra        *barra, *barraBloco;
   DIAG_ISOLADO   *blocoIso;
   int            numBloco = 1;

   //protecao
   if(!listaBlocosIsolados)return NULL;
   if(!lisBarrasIsoladas)return NULL;
   //limpa a lista de blocos isolados, mas destruindo a listas dentro (espero!)
   else{LimpaTList(listaBlocosIsolados);}
   //percorre todas as barras isoladas disponibilizadas
   while (lisBarrasIsoladas->Count > 0)
   {  //cria uma lista de barras isoladas que serão um bloco
	  blocoIso = new DIAG_ISOLADO(numBloco,"Bloco " +IntToStr(numBloco));
      //incrementa o contador de ocorencias
      //infoBlocoIsolados.n_ocorrencias++;
      //adiciona o bloco a lista de blocos
      listaBlocosIsolados->Add(blocoIso);
      //analisa uma barra
      barra = (VTBarra*) lisBarrasIsoladas->First();
	  if (barra->Tag == 1)
	  {
        blocoIso->isolado = false;
	  }
      //adiciona a barra "inicial" a lista
      blocoIso->lisBarras->Add(barra);
      for(int i = 0; i < blocoIso->lisBarras->Count; i++)
      {
         barraBloco = (VTBarra*) blocoIso->lisBarras->Items[i];
         ListaBarrasLigadas(barraBloco,blocoIso);
         //remove barra da lista de barras isoladas
         lisBarrasIsoladas->RemoveItem(barraBloco,FromBeginning);
      }
      //finaliza a edição do bloco isolado
      blocoIso->Finaliza();
      numBloco++;
   }
   return listaBlocosIsolados;
   //delete
}
//---------------------------------------------------------------------------
bool     __fastcall  TDiagBIso::SalvaArquivo(void)
{  /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
//   //variaveis locais
//   VTInfoset   *infoset;
//   DIAG_TRECHO *diagTrecho;
//   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
//
//   //infoset = NewObjInfoset(apl);
//   //infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
//   try{
//      infoset = geral->Infoset;
//      infoset->Clear();
//      for (int i = 0; i < LisCfgDiagTrecho->Count; i++)
//      {
//         diagTrecho = (DIAG_TRECHO*) LisCfgDiagTrecho->Items[i];
//         infoset->InsertNewLine();
//         infoset->AddInfo("tipo_rede", diagTrecho->tipo_rede);
//         infoset->AddInfo("nome_tipo", diagTrecho->nome_tipo);
//         infoset->AddInfo("comp_min_m", diagTrecho->comp_min_m);
//         infoset->AddInfo("comp_max_m", diagTrecho->comp_max_m);
//         infoset->AddInfo("iadm_min_a", diagTrecho->iadm_min_a);
//         infoset->AddInfo("iadm_max_a", diagTrecho->iadm_max_a);
//         //infoset->AddInfo("pu", diagTrecho->pu);
//         infoset->AddInfo("seq0_r_max_ohmkm", diagTrecho->seq0_r_max_ohmkm);
//         infoset->AddInfo("seq0_r_min_ohmkm", diagTrecho->seq0_r_min_ohmkm);
//         infoset->AddInfo("seq0_x_max_ohmkm", diagTrecho->seq0_x_max_ohmkm);
//         infoset->AddInfo("seq0_x_min_ohmkm", diagTrecho->seq0_x_min_ohmkm);
//         infoset->AddInfo("seq0_c_max_nFkm", diagTrecho->seq0_c_max_nFkm);
//         infoset->AddInfo("seq0_c_min_nFkm", diagTrecho->seq0_c_min_nFkm);
//         infoset->AddInfo("seq1_r_max_ohmkm", diagTrecho->seq1_r_max_ohmkm);
//         infoset->AddInfo("seq1_r_min_ohmkm", diagTrecho->seq1_r_min_ohmkm);
//         infoset->AddInfo("seq1_x_max_ohmkm", diagTrecho->seq1_x_max_ohmkm);
//         infoset->AddInfo("seq1_x_min_ohmkm", diagTrecho->seq1_x_min_ohmkm);
//         infoset->AddInfo("seq1_c_max_nFkm", diagTrecho->seq1_c_max_nFkm);
//         infoset->AddInfo("seq1_c_min_nFkm", diagTrecho->seq1_c_min_nFkm);
//      }
//      infoset->SaveToFile(FILE_CFG_TRECHO);
//   }
//   catch (Exception &e){return false;}
   return false;
}
//---------------------------------------------------------------------------
void     __fastcall  TDiagBIso::ShowFormCfg(void)
{
   //nada a fazer
}

//---------------------------------------------------------------------------
int      __fastcall  TDiagBIso::ShowFormCfgModal(void)
{
   //nada a fazer
   return -1;
}
//---------------------------------------------------------------------------
void __fastcall  TDiagBIso::TagBarrasComGeracao(void)
{
	VTRedes *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
	TList *lisEQPTOAUX;
	VTRede *rede;
	VTReducao *reducao;
	TList *lisRedes, *lisBAR;
	VTCorte *corte;
	VTBarra *barra;
	VTEqbar *eqbar;

	lisEQPTOAUX = new TList;
	//marca as barras com suprimentos e geradores com tag 1
	redes->LisEqbar(lisEQPTOAUX,eqptoSUPRIMENTO);
	redes->LisEqbar(lisEQPTOAUX,eqptoGERADOR);
	for (int ne = 0; ne < lisEQPTOAUX->Count; ne++)
	{
		eqbar = (VTEqbar*) lisEQPTOAUX->Items[ne];
		barra = eqbar->pbarra;
		barra->Tag = 1;
	}
	//agora trata as redes não carregadas (reducoes)
	lisRedes = redes->LisRede();
	for (int nr = 0; nr < lisRedes->Count; nr++)
	{
		rede = (VTRede*)lisRedes->Items[nr];
		if (!rede->Carregada)
		{
			for (int nred = 0 ; nred < rede->MRede->LisReducao()->Count; nred++)
			{
				reducao = (VTReducao*)(rede->MRede->LisReducao()->Items[nred]);
				if (reducao->ExisteGeracao())
				{
                    //detemina Corte da Reducao
				   corte = reducao->Corte;
				   //determina conjunto de Barras do Corte
				   lisBAR = corte->LisBarra();
				   for (int nb = 0; nb < lisBAR->Count; nb++)
				   {
					   barra = (VTBarra*)lisBAR->Items[nb];
					   barra->Tag = 1;
                   }
				}

			}
		}
	}

	if (lisEQPTOAUX)
	{
		delete lisEQPTOAUX;
	}


}
//---------------------------------------------------------------------------
int      __fastcall  TDiagBIso::Tipo(void)
{
   return(dgeBLOCOISOLADO);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagBIso::TipoAsString(void)
{
   return "Blocos Isolados";
}
//---------------------------------------------------------------------------
bool  __fastcall  TDiagBIso::VerificaRelevanciaBloco(DIAG_ISOLADO *bloco)
{  //variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList       *lisBarBloco;
   VTBarra     *barraIniBloco;
   TList       *lisLIGBAR;
   bool        adiciona = true;
   VTLigacao   *ligacao;
   VTChave     *chave;

   lisLIGBAR = new TList();
   lisBarBloco = bloco->lisBarras;
   //se for uma unica barra..
   if(lisBarBloco->Count == 1)
   {
      barraIniBloco = (VTBarra*)lisBarBloco->First();
      //verifica se tem algum eqbar ligado À barra
      if(barraIniBloco->LisEqbar()->Count < 1)
      {  //lista todas ligacoes ligadas à barra examinada
         lisLIGBAR->Clear();
         redes->LisLigacao(lisLIGBAR,barraIniBloco);
         //se tiver zero ligacoes
         if(lisLIGBAR->Count == 0) {adiciona = false;}
         //se tiver uma ligacao só
         if(lisLIGBAR->Count == 1)
         {
            ligacao = (VTLigacao*)lisLIGBAR->First();
            //se essa ligacao for uma chave
            if(ligacao->Tipo() == eqptoCHAVE)
            {
               chave = (VTChave*) ligacao;
               //se for uma chave aberta
               if(chave->Aberta)
               {  //caso só uma das barras dessa chave tenha zona..
                  if( (chave->pbarra1->zona == NULL) !=  (chave->pbarra2->zona == NULL) )
                  {
                  adiciona = false;
                  }
               }
            }
         }
      }
   }
   //deleta a lista de lig
   if(lisLIGBAR) { delete lisLIGBAR; lisLIGBAR = NULL;}

   return adiciona;
}
//---------------------------------------------------------------------------
//void     __fastcall  TDiagEqptoBIso::VerificaBlocoIsolado(VTBarra *barra)
//{
////   //DIAG_ISOLADO *diagBlocoIsolado;
////   bool  utmX, utmY;
////
////    //verifica se o comprimento do trecho esta dentro dos limites
////   if (dgBlocoIsolado.coord)
////   {
////      utmX = VerificaLimites(barra->utm.x, 0, std::numeric_limits<int>::max());
////      utmY = VerificaLimites(barra->utm.y, 0, std::numeric_limits<int>::max());
////      if(utmX || utmY)
////      {
////         AddLinhaLog(barra, opBIsoCOORDNULA);
////      }
////   }
//   //incrementa o contador de verificações
//   infoBlocoIsolados.n_verif++;
//
//}

//---------------------------------------------------------------------------
