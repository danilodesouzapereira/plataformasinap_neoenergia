//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagBarra.h"
//#include "TFormDiagEqptoBarra.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
//#define FILE_CFG_BARRA "CfgDiagEqptoBARRA"

//---------------------------------------------------------------------------
__fastcall  TDiagBarra::TDiagBarra(VTApl *apl)
{
   this->apl = apl;

   //Mostra que ainda não foi executado
   executado = false;
   //define se tem informações adicionais
   infoExtra = false;
   //cria a lista de configuracoes
   listCfg = new TList;
   //inicializa as opcoes do diagnostico
   dgBarra.coord = true;
   //cria o log_barra
   log_barra = new TStringList;


}
//---------------------------------------------------------------------------
__fastcall  TDiagBarra::~TDiagBarra(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi log
   if(log_barra){delete log_barra; log_barra = NULL;}
}

//---------------------------------------------------------------------------
void         __fastcall  TDiagBarra::AddLinhaLog(VTBarra *barra, int opDIAG_BARRA)
{  //incrementa o contador de erros
   //infoBarras.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_TRECHO
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colBar_COUNT];
   AnsiString  Separador = ";";
   //inicializa com os valores vindos do trecho
   linha[colBar_SEGMENTO]  = "-" + Separador;
   linha[colBar_REDE]      = "-" + Separador;
   linha[colBar_EQPTO]     = "Barra" + Separador;
   linha[colBar_CODIGO]    = barra->Codigo + Separador;
   linha[colBar_UTMX]      = IntToStr(barra->utm.x /100);
   linha[colBar_UTMY]      = IntToStr(barra->utm.y /100);
   if (barra)
   {
      switch (opDIAG_BARRA)
      {
         case opBarCOORDNULA:
            linha[colBar_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colBar_DESCRICAO] = "Valor de coordenadas utm nulo.";
            break;

         default:
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colBar_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_barra->AddObject(linhaPreenchida,barra);
   }
}

//---------------------------------------------------------------------------
//DIAG_TRECHO* __fastcall  TDiagBarra::CriaCfgBarraDefault(int redeSEGMENTO, AnsiString nomeTipo)
//{
//   DIAG_BARRA *diagBarra;
//
//   diagBarra = new DIAG_TRECHO;
//   diagTrecho->tipo_rede = redeSEGMENTO;
//   diagTrecho->nome_tipo = nomeTipo;
//   //poe valores default
//   diagTrecho->comp_min_m = 0;
//   diagTrecho->comp_max_m = 500;
//   diagTrecho->iadm_min_a = 0;
//   diagTrecho->iadm_max_a = 200;
//   diagTrecho->seq0_r_max_ohmkm =  25;
//   diagTrecho->seq0_r_min_ohmkm = 0;
//   diagTrecho->seq0_x_max_ohmkm = 1;
//   diagTrecho->seq0_x_min_ohmkm = 0;
//   diagTrecho->seq0_c_max_nFkm =  1;
//   diagTrecho->seq0_c_min_nFkm = 0;
//   diagTrecho->seq1_r_max_ohmkm = 25;
//   diagTrecho->seq1_r_min_ohmkm = 0;
//   diagTrecho->seq1_x_max_ohmkm = 1;
//   diagTrecho->seq1_x_min_ohmkm = 0;
//   diagTrecho->seq1_c_max_nFkm =  1;
//   diagTrecho->seq1_c_min_nFkm = 0;
//
//   return diagBarra;
//}

//---------------------------------------------------------------------------
//void     __fastcall  TDiagBarra::DiagCompri(void)
//{
//
//}
//
////---------------------------------------------------------------------------
//void     __fastcall  TDiagBarra::DiagIAdm(void)
//{
//
//}
//
////---------------------------------------------------------------------------
//void     __fastcall  TDiagBarra::DiagZSeq(void)
//{
//
//}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagBarra::Executa(void)
{  //variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTBarra     *barra;
   TList       *lisBar;

    //inicialza infos
   infoBarras.n_total = 0;
   infoBarras.n_verif = 0;
   infoBarras.n_ocorrencias = 0;
   if(log_barra)IniciaLog(log_barra);
   //if(log_barra){log_barra->Clear();}
   try
   {  //percorre todas as barras
      lisBar = redes->LisBarra();
      infoBarras.n_total = lisBar->Count;
      for(int nb = 0; nb < lisBar->Count; nb++)
      {
         barra = (VTBarra*) lisBar->Items[nb];
         temErro = false;
         //Verifica barra
         VerificaBarra(barra);
         if(temErro)infoBarras.n_ocorrencias++;
      }
      //altera o boolean mostrando que foi executado
      executado = true;
      return true;
   }
   catch(Exception &e)
   {return false;}
}

//---------------------------------------------------------------------------
//bool     __fastcall  TDiagBarra::ExisteCfgTipoRede(int redeSEGMENTO)
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
//DIAG_TRECHO*   __fastcall  TDiagBarra::Get_DiagTrecho(int redeSEGMENTO)
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
void     __fastcall  TDiagBarra::Inicia(void)
{
//   //primeiramente carrega as configs do arquivo
//   LeArquivo();
//   //verifica se falta configs pros tipos de rede existentes
//   VerificaTiposRede();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBarra::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();
   //checkListBox->Items->Add("Coordenadas nulas");
   checkListBox->Items->Add(op_BAR_TEXT[opBarCOORDNULA]);
   checkListBox->Checked[opBarCOORDNULA] = this->OpcaoDiagCoord;
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagBarra::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colBar_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {
      //limpa o log
      log->Clear();

      linha[colBar_SEGMENTO] = "Segmento" + Separador;
      linha[colBar_REDE] = "Rede" + Separador;
      linha[colBar_EQPTO] = "Eqpto" + Separador;
      linha[colBar_CODIGO] = "Código" + Separador;
      linha[colBar_TIPOERRO] = "Tipo Erro" + Separador;
      linha[colBar_UTMX] = "UTM X (m)" + Separador;
      linha[colBar_UTMY] = "UTM Y (m)" + Separador;
      linha[colBar_DESCRICAO] = "Descrição";
      //concatena os campos em uma linha
      for (int i = 0; i < colBar_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
long __fastcall TDiagBarra::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoBarras.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagBarra::PM_GetLog(void)
{
   if(executado)
   {
      return log_barra;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagBarra::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoBarras.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagBarra::PreencheListView(TListView *listView)
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
   ExtraiStrings(log_barra->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 62;
   listColumn->Caption = campos->Strings[colBar_SEGMENTO];
   //listColumn->Caption = "Segmento";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colBar_REDE];
   //listColumn->Caption = "Rede";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colBar_EQPTO];
   //listColumn->Caption = "Eqpto";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colBar_CODIGO];
   //listColumn->Caption = "Código";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 60;
   listColumn->Caption = campos->Strings[colBar_TIPOERRO];
   //listColumn->Caption = "Tipo Erro";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colBar_UTMX];
   //listColumn->Caption = "UTM X (m)";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colBar_UTMY];
   //listColumn->Caption = "UTM Y (m)";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 150;
   listColumn->Caption = campos->Strings[colBar_DESCRICAO];
   //listColumn->Caption = "Descrição";
   //listColumn->Width = ColumnHeaderWidth;

//   //coloca valor fixo de width
//   for(int nc =0; nc <listView->Columns->Count; nc++)
//   {
//      listView->Columns->Items[index]->Width = 50;
//   }

   for(int nl = 1; nl < log_barra->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_barra->Strings[nl], ";", campos);
      listItem = listView->Items->Add();
      listItem->Data = log_barra->Objects[nl];
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
bool  __fastcall  TDiagBarra::LeArquivo(void)
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
void  __fastcall  TDiagBarra::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagCoord   = checkListBox->Checked[opBarCOORDNULA];
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagBarra::SalvaArquivo(void)
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
void  __fastcall  TDiagBarra::ShowFormCfg(void)
{
//   if(formDiagEqptoBarra) formDiagEqptoBarra->Show();
//   else
//   {
//      formDiagEqptoBarra = new TFormDiagEqptoBarra(NULL, apl);
//      formDiagEqptoBarra->Inicia(this);
//   }
}

//---------------------------------------------------------------------------
int   __fastcall  TDiagBarra::ShowFormCfgModal(void)
{
//   if(formDiagEqptoBarra)
//   {
//    return formDiagEqptoBarra->ShowModal();
//   }
//   else
//   {
//      formDiagEqptoBarra = new TFormDiagEqptoBarra(NULL, apl);
//      formDiagEqptoBarra->Inicia(this);
//      return formDiagEqptoBarra->ShowModal();
//   }
//   return -1;    /* TODO : pensar melhor */
   return -1;
}
//---------------------------------------------------------------------------
int   __fastcall  TDiagBarra::Tipo(void)
{
   return(dgeBARRA);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagBarra::TipoAsString(void)
{
   return "Barras";
}
//---------------------------------------------------------------------------
//void     __fastcall  TDiagEqptoBarra::VerificaTiposRede(void)
//{
//   VTTipos    *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   TList      *lisTIPOREDE = tipos->LisTipoRede();
//   VTTipoRede *tipo_rede;
//   DIAG_TRECHO *diagTrecho;
//
//   //verifica os tipos de rede cadastrados na base
//   //em busca de dos tipos de rede que não possuem cfg no arquivo lido
//   for (int i = 0; i < lisTIPOREDE->Count; i++)
//   {
//      tipo_rede  = (VTTipoRede*) lisTIPOREDE->Items[i];
//      if( !ExisteCfgTipoRede(tipo_rede->Segmento) )
//      {  //quando encontra um tipo sem correspondente, cria um "default"
//         diagTrecho = CriaCfgTrechoDefault(tipo_rede->Segmento, tipo_rede->Codigo);
//         //adiciona à lista de configs
//         LisCfgDiagTrecho->Add(diagTrecho);
//      }
//      else
//      {
//         //faz nada
//      }
//   }
//}

//---------------------------------------------------------------------------
void  __fastcall  TDiagBarra::VerificaBarra(VTBarra *barra)
{
   //DIAG_BARRA *diagBarra;
   bool  utmX, utmY;

    //verifica se o comprimento do trecho esta dentro dos limites
   if (dgBarra.coord)
   {
      utmX = VerificaLimites(barra->utm.x, 0, std::numeric_limits<int>::max());
      utmY = VerificaLimites(barra->utm.y, 0, std::numeric_limits<int>::max());
      if(utmX || utmY)
      {
         AddLinhaLog(barra, opBarCOORDNULA);
      }
   }
   //incrementa o contador de verificações
   infoBarras.n_verif++;

}

//---------------------------------------------------------------------------
