//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagArranjo.h"
//#include "TFormDiagEqptoArranjo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
#define FILE_CFG_ARRANJO "CfgDiagEqptoArranjo"
//---------------------------------------------------------------------------
//VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
//   {
//   return (new TDiagEqptoArranjo(apl));
//   }
//---------------------------------------------------------------------------
__fastcall  TDiagArranjo::TDiagArranjo(VTApl *apl)
{
   this->apl = apl;
   //define se tem informações adicionais
   infoExtra = false;
   //cria a lista de configuracoes
   listCfg = new TList;
   //inicializa as opcoes do diagnostico
   dgArranjo.caboFase = true;
   //cria o log_arranjo
                        /* TODO : mudar para o executa? */
   log_arranjo = new TStringList;
   //Mostra que ainda não foi executado
   executado = false;
}

//---------------------------------------------------------------------------
__fastcall  TDiagArranjo::~TDiagArranjo(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi log
   if(log_arranjo){delete log_arranjo; log_arranjo = NULL;}
}

//---------------------------------------------------------------------------
void __fastcall  TDiagArranjo::AddLinhaLog(VTArranjo *arranjo, int opDIAG_ARRANJO)
{  //incrementa o contador de erros
   //infoArranjos.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_ARRANJO
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colArj_COUNT];
   AnsiString  Separador = ";";
   //inicializa com os valores vindos do arranjo
   linha[colArj_EQPTO]     = "Arranjo" + Separador;
   linha[colArj_CODIGO]    = arranjo->Codigo + Separador;
   if (arranjo)
   {
      switch (opDIAG_ARRANJO)
      {
         case opArjCABOFASES:
            linha[colArj_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colArj_DESCRICAO] = "Cabos diferentes em cada fase";

            break;
         default:
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colArj_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_arranjo->Add(linhaPreenchida);
   }
}

//---------------------------------------------------------------------------
//DIAG_ARRANJO* __fastcall  TDiagArranjo::CriaCfgArranjoDefault(int redeSEGMENTO, AnsiString nomeTipo)
//{
//   DIAG_ARRANJO *diagArranjo;
//
//   diagArranjo = new DIAG_ARRANJO;
//   diagArranjo->tipo_rede = redeSEGMENTO;
//   diagArranjo->nome_tipo = nomeTipo;
//   //poe valores default
//   diagArranjo->comp_min_m = 0;
//   diagArranjo->comp_max_m = 500;
//   diagArranjo->iadm_min_a = 0;
//   diagArranjo->iadm_max_a = 200;
//   diagArranjo->seq0_r_max_ohmkm =  25;
//   diagArranjo->seq0_r_min_ohmkm = 0;
//   diagArranjo->seq0_x_max_ohmkm = 1;
//   diagArranjo->seq0_x_min_ohmkm = 0;
//   diagArranjo->seq0_c_max_nFkm =  1;
//   diagArranjo->seq0_c_min_nFkm = 0;
//   diagArranjo->seq1_r_max_ohmkm = 25;
//   diagArranjo->seq1_r_min_ohmkm = 0;
//   diagArranjo->seq1_x_max_ohmkm = 1;
//   diagArranjo->seq1_x_min_ohmkm = 0;
//   diagArranjo->seq1_c_max_nFkm =  1;
//   diagArranjo->seq1_c_min_nFkm = 0;
//
//   return diagArranjo;
//}
//---------------------------------------------------------------------------
bool  __fastcall  TDiagArranjo::Executa(void)
{  //variaveis locais
   VTArranjos   *arranjos = (VTArranjos*) apl->GetObject(__classid(VTArranjos));
   VTArranjo    *arranjo;

   //inicialza infos
   infoArranjos.n_total = 0;
   infoArranjos.n_verif = 0;
   infoArranjos.n_ocorrencias = 0;
   //if(log_arranjo){log_arranjo->Clear();}
   if(log_arranjo)IniciaLog(log_arranjo);
   try
   {
      infoArranjos.n_total = arranjos->LisArranjo()->Count;
      //percorre todas os arranjos
      for(int na = 0; na < arranjos->LisArranjo()->Count; na++)
      {
         arranjo = (VTArranjo*) arranjos->LisArranjo()->Items[na];
         temErro = false;
         if(arranjo){VerificaArranjo(arranjo);}
         if(temErro)infoArranjos.n_ocorrencias++;
      }
      //altera o boolean mostrando que foi executado
      executado = true;
      return true;
   }
   catch(Exception &e)
   {return false;}
}

//---------------------------------------------------------------------------
//bool     __fastcall  TDiagArranjo::ExisteCfgTipoRede(int redeSEGMENTO)
//{  //variaveis locais
//   DIAG_ARRANJO *diagArranjo;
//
//   for(int nt = 0; nt < listCfg->Count; nt++)
//   {
//      diagArranjo = (DIAG_ARRANJO*) listCfg->Items[nt];
//      if (diagArranjo->tipo_rede == redeSEGMENTO) {return true;}
//   }
//   return false;
//}

//---------------------------------------------------------------------------
//DIAG_ARRANJO*   __fastcall  TDiagArranjo::Get_DiagArranjo(int redeSEGMENTO)
//{  //variaveis locais
//   DIAG_ARRANJO *diagArranjo;
//
//   for(int i = 0 ; i < listCfg->Count; i++)
//   {
//      diagArranjo = (DIAG_ARRANJO*) listCfg->Items[i];
//      if(redeSEGMENTO == diagArranjo->tipo_rede){return diagArranjo;}
//   }
//   return NULL;
//}

//---------------------------------------------------------------------------
void  __fastcall  TDiagArranjo::Inicia(void)
{
   //primeiramente carrega as configs do arquivo
   //LeArquivo();
   //verifica se falta configs pros tipos de rede existentes
   //VerificaTiposRede();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagArranjo::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();
   checkListBox->Items->Add("Cabos das Fases");
   checkListBox->Checked[opArjCABOFASES] = this->OpcaoDiagCaboFase;
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagArranjo::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colArj_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {  //limpa o log
      log->Clear();
      //cria a linha com os nomes das colunas
      linha[colArj_EQPTO] = "Eqpto" + Separador;
      linha[colArj_CODIGO] = "Código" + Separador;
      linha[colArj_TIPOERRO] = "Tipo Erro" + Separador;
      linha[colArj_DESCRICAO] = "Descrição";
      //concatena os campos em uma linha
      for (int i = 0; i < colArj_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
long __fastcall TDiagArranjo::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoArranjos.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagArranjo::PM_GetLog(void)
{
   if(executado)
   {
      return log_arranjo;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagArranjo::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoArranjos.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}

//---------------------------------------------------------------------------
void     __fastcall  TDiagArranjo::PreencheListView(TListView *listView)
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
   ExtraiStrings(log_arranjo->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colArj_EQPTO];
   //listColumn->Caption = "Equipamento";
   listColumn = listView->Columns->Add();
   listColumn->Width = 50;
   listColumn->Caption = campos->Strings[colArj_CODIGO];
   //listColumn->Caption = "Codigo";
   listColumn = listView->Columns->Add();
   listColumn->Width = 60;
   listColumn->Caption = campos->Strings[colArj_TIPOERRO];
   //listColumn->Caption = "Tipo Erro";
   listColumn = listView->Columns->Add();
   listColumn->Width = 300;
   listColumn->Caption = campos->Strings[colArj_DESCRICAO];
   //listColumn->Caption = "Descrição";

//   //coloca valor fixo de width
//   for(int nc =0; nc <listView->Columns->Count; nc++)
//   {
//      listView->Columns->Items[index]->Width = 50;
//   }

   for(int nl = 1; nl < log_arranjo->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_arranjo->Strings[nl], ";", campos);

      listItem = listView->Items->Add();
      listItem->Data = log_arranjo->Objects[nl];
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
bool     __fastcall  TDiagArranjo::LeArquivo(void)
{  //variaveis locais
//   VTInfoset   *infoset;
//   DIAG_ARRANJO *diagArranjo;
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
//      if(infoset->LoadFromFile(FILE_CFG_ARRANJO))
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
//               diagArranjo = new DIAG_ARRANJO;
//               infoset->GetInfo("tipo_rede", diagArranjo->tipo_rede);
//               infoset->GetInfo("nome_tipo", diagArranjo->nome_tipo);
//               infoset->GetInfo("comp_min_m", diagArranjo->comp_min_m);
//               infoset->GetInfo("comp_max_m", diagArranjo->comp_max_m);
//               infoset->GetInfo("iadm_min_a", diagArranjo->iadm_min_a);
//               infoset->GetInfo("iadm_max_a", diagArranjo->iadm_max_a);
//               //infoset->AddInfo("pu", diagArranjo->pu);
//               infoset->GetInfo("seq0_r_max_ohmkm", diagArranjo->seq0_r_max_ohmkm);
//               infoset->GetInfo("seq0_r_min_ohmkm", diagArranjo->seq0_r_min_ohmkm);
//               infoset->GetInfo("seq0_x_max_ohmkm", diagArranjo->seq0_x_max_ohmkm);
//               infoset->GetInfo("seq0_x_min_ohmkm", diagArranjo->seq0_x_min_ohmkm);
//               infoset->GetInfo("seq0_c_max_nFkm", diagArranjo->seq0_c_max_nFkm);
//               infoset->GetInfo("seq0_c_min_nFkm", diagArranjo->seq0_c_min_nFkm);
//               infoset->GetInfo("seq1_r_max_ohmkm", diagArranjo->seq1_r_max_ohmkm);
//               infoset->GetInfo("seq1_r_min_ohmkm", diagArranjo->seq1_r_min_ohmkm);
//               infoset->GetInfo("seq1_x_max_ohmkm", diagArranjo->seq1_x_max_ohmkm);
//               infoset->GetInfo("seq1_x_min_ohmkm", diagArranjo->seq1_x_min_ohmkm);
//               infoset->GetInfo("seq1_c_max_nFkm", diagArranjo->seq1_c_max_nFkm);
//               infoset->GetInfo("seq1_c_min_nFkm", diagArranjo->seq1_c_min_nFkm);
//               listCfg->Add(diagArranjo);
//               //caso o tipo rede seja 0 (== PADRAO)
//               if(tipoRede == 0){temPadrao = true;}
//            }
//         }
//      }//caso não exista o arquivo
//      else
//      {  //cria configs para todos os
//         VerificaTiposRede();
//         //cria o tipo "default"
//         diagArranjo = CriaCfgArranjoDefault(0, "*Padrão*");
//         //define como padrao
//         //diagArranjoPADRAO = diagArranjo;
//         //adiciona à lista de configs
//         listCfg->Add(diagArranjo);
//         temPadrao = true;
//         //e cria o arquivo
//         SalvaArquivo();
//      }
//      if (!temPadrao)
//      {  //cria o tipo "default"
//         diagArranjo = CriaCfgArranjoDefault(0, "*Padrão*");
//         //define como padrao
//         //diagArranjoPADRAO = diagArranjo;
//         //adiciona à lista de configs
//         listCfg->Add(diagArranjo);
//         temPadrao = true;
//      }
//   return true;
//   } catch (Exception &e) {return false;}
   return false;}

//---------------------------------------------------------------------------
void  __fastcall  TDiagArranjo::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagCaboFase  = checkListBox->Checked[opArjCABOFASES];
}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagArranjo::SalvaArquivo(void)
{  /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
//   //variaveis locais
//   VTInfoset   *infoset;
//   DIAG_ARRANJO *diagArranjo;
//   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
//
//   //infoset = NewObjInfoset(apl);
//   //infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
//   try{
//      infoset = geral->Infoset;
//      infoset->Clear();
//      for (int i = 0; i < listCfg->Count; i++)
//      {
//         diagArranjo = (DIAG_ARRANJO*) listCfg->Items[i];
//         infoset->InsertNewLine();
//         infoset->AddInfo("tipo_rede", diagArranjo->tipo_rede);
//         infoset->AddInfo("nome_tipo", diagArranjo->nome_tipo);
//         infoset->AddInfo("comp_min_m", diagArranjo->comp_min_m);
//         infoset->AddInfo("comp_max_m", diagArranjo->comp_max_m);
//         infoset->AddInfo("iadm_min_a", diagArranjo->iadm_min_a);
//         infoset->AddInfo("iadm_max_a", diagArranjo->iadm_max_a);
//         //infoset->AddInfo("pu", diagArranjo->pu);
//         infoset->AddInfo("seq0_r_max_ohmkm", diagArranjo->seq0_r_max_ohmkm);
//         infoset->AddInfo("seq0_r_min_ohmkm", diagArranjo->seq0_r_min_ohmkm);
//         infoset->AddInfo("seq0_x_max_ohmkm", diagArranjo->seq0_x_max_ohmkm);
//         infoset->AddInfo("seq0_x_min_ohmkm", diagArranjo->seq0_x_min_ohmkm);
//         infoset->AddInfo("seq0_c_max_nFkm", diagArranjo->seq0_c_max_nFkm);
//         infoset->AddInfo("seq0_c_min_nFkm", diagArranjo->seq0_c_min_nFkm);
//         infoset->AddInfo("seq1_r_max_ohmkm", diagArranjo->seq1_r_max_ohmkm);
//         infoset->AddInfo("seq1_r_min_ohmkm", diagArranjo->seq1_r_min_ohmkm);
//         infoset->AddInfo("seq1_x_max_ohmkm", diagArranjo->seq1_x_max_ohmkm);
//         infoset->AddInfo("seq1_x_min_ohmkm", diagArranjo->seq1_x_min_ohmkm);
//         infoset->AddInfo("seq1_c_max_nFkm", diagArranjo->seq1_c_max_nFkm);
//         infoset->AddInfo("seq1_c_min_nFkm", diagArranjo->seq1_c_min_nFkm);
//      }
//      infoset->SaveToFile(FILE_CFG_ARRANJO);
//      return true;
//   }
//   catch (Exception &e){return false;}
   return false;}
//---------------------------------------------------------------------------
void     __fastcall  TDiagArranjo::ShowFormCfg(void)
{
//   if(formDiagEqptoArranjo) formDiagEqptoArranjo->Show();
//   else
//   {
//      formDiagEqptoArranjo = new TFormDiagEqptoArranjo(NULL, apl);
//      formDiagEqptoArranjo->Inicia(this);
//      formDiagEqptoArranjo->Show();
//   }
}

//---------------------------------------------------------------------------
int      __fastcall  TDiagArranjo::ShowFormCfgModal(void)
{
//   if(formDiagEqptoArranjo)
//   {
//    return formDiagEqptoArranjo->ShowModal();
//   }
//   else
//   {
//      formDiagEqptoArranjo = new TFormDiagEqptoArranjo(NULL, apl);
//      formDiagEqptoArranjo->Inicia(this);
//      return formDiagEqptoArranjo->ShowModal();
//   }
//   return -1;    /* TODO : pensar melhor */
   return -1;
}
//---------------------------------------------------------------------------
int      __fastcall  TDiagArranjo::Tipo(void)
{
   return(dgeARRANJO);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagArranjo::TipoAsString(void)
{
   return "Arranjos";
}

//---------------------------------------------------------------------------
//void     __fastcall  TDiagEqptoArranjo::VerificaTiposRede(void)
//{
//   VTTipos    *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   TList      *lisTIPOREDE = tipos->LisTipoRede();
//   VTTipoRede *tipo_rede;
//   DIAG_ARRANJO *diagArranjo;
//
//   //verifica os tipos de rede cadastrados na base
//   //em busca de dos tipos de rede que não possuem cfg no arquivo lido
//   for (int i = 0; i < lisTIPOREDE->Count; i++)
//   {
//      tipo_rede  = (VTTipoRede*) lisTIPOREDE->Items[i];
//      if( !ExisteCfgTipoRede(tipo_rede->Segmento) )
//      {  //quando encontra um tipo sem correspondente, cria um "default"
//         diagArranjo = CriaCfgArranjoDefault(tipo_rede->Segmento, tipo_rede->Codigo);
//         //adiciona à lista de configs
//         listCfg->Add(diagArranjo);
//      }
//      else
//      {
//         //faz nada
//      }
//   }
//}

//---------------------------------------------------------------------------
void     __fastcall  TDiagArranjo::VerificaArranjo(VTArranjo *arranjo)
{

   //verifica se as fases do arranjo são iguais
   if (dgArranjo.caboFase){VerificaOpCaboFase(arranjo, NULL);}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagArranjo::VerificaOpCaboFase(VTArranjo *arranjo, DIAG_ARRANJO *diagArranjo)
{  //variaveis locais
   bool fase_A_B, fase_A_C, fase_B_C;


   //inicializa os booleans
   fase_A_B = fase_A_C = fase_B_C = true;
   //nesse caso simples o diagArranjo está NULL, verificação simples sem necessidade de parametros
   if(!diagArranjo)
   {  //caso o arrranho seja formado por suporte + cabo
      if(arranjo->TipoCfg == arranjoCABOxFASE)
      {  //verifica se os pares são iguais
         fase_A_B = (arranjo->fase[indFASE_A].cabo ==  arranjo->fase[indFASE_B].cabo);
         fase_A_C = (arranjo->fase[indFASE_A].cabo ==  arranjo->fase[indFASE_C].cabo);
         fase_B_C = (arranjo->fase[indFASE_B].cabo ==  arranjo->fase[indFASE_C].cabo);
         //se houver algum par diferente adiciona erro ao log
         if( (!fase_A_B) || (!fase_A_C) || (!fase_B_C)) {AddLinhaLog(arranjo, opArjCABOFASES);}
      }
   }
   //incrementa o contador de verificações
   infoArranjos.n_verif++;
}

//---------------------------------------------------------------------------
