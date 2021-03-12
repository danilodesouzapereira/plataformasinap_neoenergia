//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagCapacitor.h"
#include "TFormCatalogoCap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
#define FILE_CFG_CAPACITOR "CfgDiagEqptoCapacitor"
//---------------------------------------------------------------------------
//VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
//   {
//   return (new TDiagEqptoCapacitor(apl));
//   }
//---------------------------------------------------------------------------
__fastcall  TDiagCapacitor::TDiagCapacitor(VTApl *apl)
{
   this->apl = apl;
   //define se tem informações adicionais
   infoExtra = false;
   //cria a lista de configuracoes
   listCfg = new TList;
   //inicializa as opcoes do diagnostico
   dgCapacitor.existeNoCatalogo = true;
   dgCapacitor.q_mvar = true;
//   dgCapacitor.isolado = true;
   //cria o log_capacitor
                        /* TODO : mudar para o executa? */
   log_capacitor = new TStringList;
   //Mostra que ainda não foi executado
   executado = false;


}
//---------------------------------------------------------------------------
__fastcall  TDiagCapacitor::~TDiagCapacitor(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi log
   if(log_capacitor){delete log_capacitor; log_capacitor = NULL;}
   //destroi form
   if(formCatalogoCap){delete formCatalogoCap; formCatalogoCap = NULL;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::AddLinhaLog(VTCapacitor *capacitor,int opDIAG_CAPACITOR, DIAG_CAPACITOR *diagCapacitor)
{  //incrementa o contador de erros
   //infoCapacitores.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_CAPACITOR
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colCap_COUNT];
   AnsiString  Separador = ";";
   //inicializa com os valores vindos do capacitor
   //linha[colCap_SEGMENTO]  = capacitor->rede->TipoRede->Codigo + Separador;
   //linha[colCap_REDE]      = capacitor->rede->Codigo + Separador;
   //linha[colLog_EQPTO]     = capacitor->TipoEqpto_AsString(capacitor->Tipo()) + Separador;
   linha[colCap_EQPTO]     = "Capacitor" + Separador;
   linha[colCap_CODIGO]    = capacitor->Codigo + Separador;
   linha[colCap_CODBARRA]  = capacitor->pbarra->Codigo + Separador;

   if (capacitor)
   {
      switch (opDIAG_CAPACITOR)
      {
         case opCapCATALOGO:
            linha[colCap_TIPOERRO]  = str_TIPOERRO_ERRO + Separador;
            linha[colCap_DESCRICAO] = "Banco de capacitores não encontrado no catálogo.";
            break;

         case opCapQMVAr:
            linha[colCap_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colCap_DESCRICAO] = "Valor de potência diferente dos disponíveis na empresa.";
            break;

//		case opCapISO:
//			linha[colCap_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
//			linha[colCap_DESCRICAO] = "Capacitor isolado.";
//			break;

         default:
            linha[colCap_TIPOERRO]  = " - " + Separador;
            linha[colCap_DESCRICAO] = " - ";
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colCap_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_capacitor->AddObject(linhaPreenchida, capacitor);
   }
}

//---------------------------------------------------------------------------
DIAG_CAPACITOR* __fastcall  TDiagCapacitor::CriaCfgCapacitorDefault(int tipo_cap, AnsiString nomeTipo)
{
   DIAG_CAPACITOR *diagCapacitor;

   diagCapacitor = new DIAG_CAPACITOR;
   diagCapacitor->id = tipo_cap;
   diagCapacitor->codigo = nomeTipo;
   //poe valores default
   diagCapacitor->Q_MVAr = 0.3;

   return diagCapacitor;
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCapacitor::Executa(void)
{  //variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTCapacitor *capacitor;
   TList       *listCap = new TList();

   //inicialza infos
   infoCapacitores.n_total = 0;
   infoCapacitores.n_verif = 0;
   infoCapacitores.n_ocorrencias = 0;
   //if(log_capacitor){log_capacitor->Clear();}
   if(log_capacitor)IniciaLog(log_capacitor);
   try
   {
      redes->LisEqbar(listCap,eqptoCAPACITOR);
      infoCapacitores.n_total = listCap->Count;
      //um cast na ligacao
      for(int nc = 0; nc < listCap->Count; nc++)
      {
         capacitor = (VTCapacitor*) listCap->Items[nc];
         temErro = false;
         //verifica o capacitor
         VerificaCapacitor(capacitor);
         if(temErro)infoCapacitores.n_ocorrencias++;
      }
      //altera o boolean mostrando que foi executado
      executado = true;
      //deleta a lista de capacitores
      if(listCap){delete listCap; listCap = NULL;}
      return true;
   }
   catch(Exception &e)
   {
      //deleta a lista de capacitores
      if(listCap){delete listCap; listCap = NULL;}
      return false;
   }
}

//---------------------------------------------------------------------------
//bool     __fastcall  TDiagCapacitor::ExisteCfgTipoCap(int tipo_cap)
//{  //variaveis locais
//   DIAG_CAPACITOR *diagCapacitor;
//
//   for(int nt = 0; nt < listCfg->Count; nt++)
//   {
//      diagCapacitor = (DIAG_CAPACITOR*) listCfg->Items[nt];
//      if (diagCapacitor->tipo_cap == tipo_cap) {return true;}
//   }
//   return false;
//}

//---------------------------------------------------------------------------
DIAG_CAPACITOR*   __fastcall  TDiagCapacitor::Get_DiagCapacitor(int tipo_cap)
{  //variaveis locais
   DIAG_CAPACITOR *diagCapacitor;

   for(int i = 0 ; i < listCfg->Count; i++)
   {
      diagCapacitor = (DIAG_CAPACITOR*) listCfg->Items[i];
      if(tipo_cap == diagCapacitor->id){return diagCapacitor;}
   }
   return NULL;
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::Inicia(void)
{
   //primeiramente carrega as configs do arquivo
   LeArquivo();
   //verifica se falta configs pros tipos de rede existentes
   //VerificaTiposRede();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();

   checkListBox->Items->Add(op_CAP_TEXT[opCapCATALOGO]);
   checkListBox->Checked[opCapCATALOGO] = this->OpcaoDiagExisteNoCatalogo;

   checkListBox->Items->Add(op_CAP_TEXT[opCapQMVAr]);
   checkListBox->Checked[opCapQMVAr] = this->OpcaoDiagQMVAr;

//   checkListBox->Items->Add(op_CAP_TEXT[opCapISO]);
//   checkListBox->Checked[opCapISO] = this->OpcaoDiagIsolado;
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colCap_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {
      //limpa o log
      log->Clear();
      //linha[colTch_SEGMENTO] = "Segmento" + Separador;
      //linha[colTch_REDE] = "Rede" + Separador;
      linha[colCap_EQPTO] = "Eqpto" + Separador;
      linha[colCap_CODIGO] = "Código" + Separador;
      linha[colCap_TIPOERRO] = "Tipo Erro" + Separador;
      linha[colCap_CODBARRA] = "Barra" + Separador;
      linha[colCap_DESCRICAO] = "Descrição";
      //concatena os campos em uma linha
      for (int i = 0; i < colCap_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
long __fastcall  TDiagCapacitor::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoCapacitores.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagCapacitor::PM_GetLog(void)
{
   if(executado)
   {
      return log_capacitor;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagCapacitor::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoCapacitores.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}
//---------------------------------------------------------------------------
void     __fastcall  TDiagCapacitor::PreencheListView(TListView *listView)
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
   ExtraiStrings(log_capacitor->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 60;
   listColumn->Caption = campos->Strings[colCap_EQPTO];
   //listColumn->Caption = "Eqpto";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 60;
   listColumn->Caption = campos->Strings[colCap_CODIGO];
   //listColumn->Caption = "Código";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 80;
   listColumn->Caption = campos->Strings[colCap_TIPOERRO];
  // listColumn->Caption = "Tipo Erro";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colCap_CODBARRA];
   //listColumn->Caption = "Barra";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 300;
   listColumn->Caption = campos->Strings[colCap_DESCRICAO];
   //listColumn->Caption = "Descrição";
   //listColumn->Width = ColumnHeaderWidth;

   //coloca valor fixo de width
//   for(int nc =0; nc <listView->Columns->Count; nc++)
//   {
//      listView->Columns->Items[index]->Width = 50;
//   }

   for(int nl = 1; nl < log_capacitor->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_capacitor->Strings[nl], ";", campos);
      listItem = listView->Items->Add();
      listItem->Data = log_capacitor->Objects[nl];
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
bool  __fastcall  TDiagCapacitor::LeArquivo(void)
{  //variaveis locais
   VTInfoset   *infoset;
   DIAG_CAPACITOR *diagCapacitor;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   bool        temPadrao = false;

   try {
      //cria novo infoset para ler o arquivo de cfg
      infoset = geral->Infoset;
      infoset->Clear();
      //verifica se é possivel ler o arquivo
      if(infoset->LoadFromFile(FILE_CFG_CAPACITOR))
      {  //varre todo o arquivo lido
         for (int i = 0; i < infoset->Count; i++)
         {
            infoset->SelectLine(i);
            diagCapacitor = new DIAG_CAPACITOR;
            infoset->GetInfo("id", diagCapacitor->id);
            infoset->GetInfo("codigo", diagCapacitor->codigo);
            infoset->GetInfo("Q_MVAr", diagCapacitor->Q_MVAr);
            listCfg->Add(diagCapacitor);
            //caso o tipo rede seja 0 (== PADRAO)
            if(diagCapacitor->id == 0){temPadrao = true;}
         }
      }
      //caso não exista o arquivo
      else
      {  //cria o tipo "default"
         diagCapacitor = CriaCfgCapacitorDefault(0, "*Padrão*");
         //define como padrao
         //adiciona à lista de configs
         listCfg->Add(diagCapacitor);
         temPadrao = true;
         //e cria o arquivo
         SalvaArquivo();
      }
      if (!temPadrao)
      {  //cria o tipo "default"
         diagCapacitor = CriaCfgCapacitorDefault(0, "*Padrão*");
         //adiciona à lista de configs
         listCfg->Add(diagCapacitor);
         temPadrao = true;
      }
   return true;
   } catch (Exception &e) {return false;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagExisteNoCatalogo  = checkListBox->Checked[opCapCATALOGO];
   this->OpcaoDiagQMVAr             = checkListBox->Checked[opCapQMVAr];
//   this->OpcaoDiagIsolado           = checkListBox->Checked[opCapISO];
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCapacitor::SalvaArquivo(void)
{  /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
   //variaveis locais
   VTInfoset   *infoset;
   DIAG_CAPACITOR *diagCapacitor;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   try{
      infoset = geral->Infoset;
      infoset->Clear();
      for (int i = 0; i < listCfg->Count; i++)
      {
         diagCapacitor = (DIAG_CAPACITOR*) listCfg->Items[i];
         infoset->InsertNewLine();
         infoset->AddInfo("id", diagCapacitor->id);
         infoset->AddInfo("codigo", diagCapacitor->codigo);
         infoset->AddInfo("Q_MVAr", diagCapacitor->Q_MVAr);
      }
      infoset->SaveToFile(FILE_CFG_CAPACITOR);
      return true;
   }
   catch (Exception &e){return false;}
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::ShowFormCfg(void)
{
   if(formCatalogoCap) formCatalogoCap->Show();
   else
   {
      formCatalogoCap = new TFormCatalogoCap(NULL, apl);
      formCatalogoCap->Inicia(this);
      formCatalogoCap->Show();
   }
}

//---------------------------------------------------------------------------
int   __fastcall  TDiagCapacitor::ShowFormCfgModal(void)
{
//   if(formCatalogoCap)
//   {
//    return formCatalogoCap->ShowModal();
//   }
//   else
//   {
//      formCatalogoCap = new TFormDiagEqptoCapacitor(NULL, apl);
//      formCatalogoCap->Inicia(this);
//      return formCatalogoCap->ShowModal();
//   }
//   return -1;    /* TODO : pensar melhor */
   return false;
}
//---------------------------------------------------------------------------
int   __fastcall  TDiagCapacitor::Tipo(void)
{
   return(dgeCAPACITOR);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagCapacitor::TipoAsString(void)
{
   return "Capacitores";
}

//---------------------------------------------------------------------------
//void   __fastcall  TDiagEqptoCapacitor::VerificaTiposRede(void)
//{
//   VTTipos    *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   TList      *lisTIPOREDE = tipos->LisTipoRede();
//   VTTipoRede *tipo_rede;
//   DIAG_CAPACITOR *diagCapacitor;
//
//   //verifica os tipos de rede cadastrados na base
//   //em busca de dos tipos de rede que não possuem cfg no arquivo lido
//   for (int i = 0; i < lisTIPOREDE->Count; i++)
//   {
//      tipo_rede  = (VTTipoRede*) lisTIPOREDE->Items[i];
//      if( !ExisteCfgTipoRede(tipo_rede->Segmento) )
//      {  //quando encontra um tipo sem correspondente, cria um "default"
//         diagCapacitor = CriaCfgCapacitorDefault(tipo_rede->Segmento, tipo_rede->Codigo);
//         //adiciona à lista de configs
//         LisCfgDiagCapacitor->Add(diagCapacitor);
//      }
//      else
//      {
//         //faz nada
//      }
//   }
//}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::VerificaCapacitor(VTCapacitor *capacitor)
{  //variaveis locais
   DIAG_CAPACITOR *diagCapacitor;

   //verifica se existe no catalogo
   diagCapacitor = VerificaOpExisteNoCatalogo(capacitor);
   //caso encontre no catalogo, pode-se fazer outras verificações
   if(diagCapacitor)
   {  //verifica se os valores são compativeis com o cabo padrao encontrado
      if (dgCapacitor.q_mvar)VerificaOpQ_MVAr(capacitor, diagCapacitor);
   }
   //verifica se o cap está isolado
//   if (dgCapacitor.isolado)VerificaOpIsolado(capacitor);
   //incrementa o num de caps verificados
   infoCapacitores.n_verif++;
}

//---------------------------------------------------------------------------
//Provavel problema de desempenho!
DIAG_CAPACITOR*  __fastcall  TDiagCapacitor::VerificaOpExisteNoCatalogo(VTCapacitor *capacitor)
{  //variaveis locais
   DIAG_CAPACITOR *diagCapacitor;
   bool      encontrou = false;

   for(int ni = 0; ni < listCfg->Count;ni++)
   {

      diagCapacitor =  (DIAG_CAPACITOR*)listCfg->Items[ni];
      if(IsDoubleZero((capacitor->Q_mvar) - (diagCapacitor->Q_MVAr)))
         {
         encontrou = true;
         return diagCapacitor;
         }
//      if(diagCapacitor->nome_tipo.AnsiCompareIC(capacitor->Codigo) == 0)
//      {  //correspondencia exata
//         encontrou = true;
//         return diagCapacitor;
//      }
//      else
//      {
//         if(diagCapacitor->nome_tipo.AnsiCompareIC(capacitor->Codigo) < 0)
//         {
//            if(capacitor->Codigo.Pos(diagCapacitor->nome_tipo) > 0)
//            {//contem a string
//               encontrou = true;
//               return diagCapacitor;
//            }
//         }
//         else
//         {
//            if(diagCapacitor->nome_tipo.Pos(capacitor->Codigo) > 0)
//            {//contem a string
//               encontrou = true;
//               return diagCapacitor;
//            }
//         }
//      }
   }
   //se não encontrou
   if(!encontrou)
   {
      if (dgCapacitor.existeNoCatalogo)AddLinhaLog(capacitor, opCapCATALOGO);
   }
   return NULL;
}
//---------------------------------------------------------------------------
//void  __fastcall  TDiagCapacitor::VerificaOpIsolado(VTCapacitor *capacitor)
//{  //variaveis locais
//   VTBarra  *barra;
//
//   barra = capacitor->pbarra;
//   if(barra->zona == NULL)
//   {
//	   AddLinhaLog(capacitor, opCapISO);
//   }
//
//}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCapacitor::VerificaOpQ_MVAr(VTCapacitor *capacitor, DIAG_CAPACITOR *diagCapacitor)
{  //variaveis locais
   double a, b;

   //verifica os valores do raio [m]
   a = capacitor->Q_mvar;
   b = diagCapacitor->Q_MVAr;
   if(!IsDoubleZero(a - b,1e-3))
   {
      AddLinhaLog(capacitor, opCapQMVAr,diagCapacitor);
   }
}

//---------------------------------------------------------------------------
