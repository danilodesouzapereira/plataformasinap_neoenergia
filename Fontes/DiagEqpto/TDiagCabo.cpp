//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagCabo.h"
#include "TFormCatalogoCab.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
#define FILE_CFG_CABO "CfgDiagEqptoCabo"
//---------------------------------------------------------------------------
//VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
//   {
//   return (new TDiagEqptoCabo(apl));
//   }
//---------------------------------------------------------------------------
__fastcall  TDiagCabo::TDiagCabo(VTApl *apl)
{
   this->apl = apl;
   //define se tem informações adicionais
   infoExtra = false;
   //cria a lista de configuracoes
   listCfg = new TList;
   //inicializa as opcoes do diagnostico
   dgCabo.existeNoCatalogo = true;
   dgCabo.raio = true;
   dgCabo.resist = true;
   dgCabo.rmg = true;
   //cria o log_cabo
                        /* TODO : mudar para o executa? */
   log_cabo = new TStringList;
   //Mostra que ainda não foi executado
   executado = false;


}
//---------------------------------------------------------------------------
__fastcall  TDiagCabo::~TDiagCabo(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi log
   if(log_cabo){delete log_cabo; log_cabo = NULL;}
   //destroi form
   if(formCatalogoCab){delete formCatalogoCab; formCatalogoCab = NULL;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::AddLinhaLog(VTCabo *cabo, int opDIAG_CABO, DIAG_CABO *diagCabo)
{  //incrementa o contador de erros
   //infoCabos.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_CABO
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colCab_COUNT];
   AnsiString  Separador = ";";
   //inicializa com os valores vindos do cabo
   linha[colCab_EQPTO]     = "Cabo" + Separador;
   linha[colCab_CODIGO]    = cabo->Codigo + Separador;
   if (cabo)
   {
      switch (opDIAG_CABO)
      {
         case opCabCATALOGO:
            linha[colCab_TIPOERRO]  = str_TIPOERRO_ERRO + Separador;
            linha[colCab_PADRAO]    = " - " + Separador;
            linha[colCab_DESCRICAO] = "Cabo não encontrado no catálogo."+ Separador;;
            linha[colCab_VAL_PADRAO]= " - " + Separador;
            linha[colCab_VAL_CABO]  = " - " + Separador;
            break;

         case opCabRAIO:
            linha[colCab_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            if(diagCabo)linha[colCab_PADRAO]    = diagCabo->codigo + Separador;
            linha[colCab_DESCRICAO] = "Raio diferente ao do cabo comparado."+ Separador;;
            if(diagCabo)linha[colCab_VAL_PADRAO]= DoubleToStr("%4.3f",diagCabo->raio_m) + Separador;
            if(cabo)linha[colCab_VAL_CABO]  = DoubleToStr("%4.3f",cabo->raio) + Separador;
            break;

         case opCabRESIST:
            linha[colCab_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            if(diagCabo)linha[colCab_PADRAO]    = diagCabo->codigo + Separador;
            linha[colCab_DESCRICAO] = "Resistência diferente ao do cabo comparado."+ Separador;;
            if(diagCabo)linha[colCab_VAL_PADRAO]= DoubleToStr("%4.3f",diagCabo->r_ohm_km) + Separador;
            if(cabo)linha[colCab_VAL_CABO]  = DoubleToStr("%4.3f",cabo->r) + Separador;
            break;

         case opCabRMG:
            linha[colCab_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            if(diagCabo)linha[colCab_PADRAO]    = diagCabo->codigo + Separador;
            linha[colCab_DESCRICAO] = "RMG diferente ao do cabo comparado."+ Separador;;
            if(diagCabo)linha[colCab_VAL_PADRAO]= DoubleToStr("%4.3f",diagCabo->gmr_m) + Separador;
            if(cabo)linha[colCab_VAL_CABO]  = DoubleToStr("%4.3f",cabo->gmr) + Separador;
            break;

         default:
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colCab_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_cabo->Add(linhaPreenchida);
   }
}

//---------------------------------------------------------------------------
DIAG_CABO* __fastcall  TDiagCabo::CriaCfgCaboDefault(int id, AnsiString codigo)
{
   DIAG_CABO *diagCabo;

   diagCabo = new DIAG_CABO;
   diagCabo->id = id;
   diagCabo->cfg_cabo = caboZ0Z1;
   diagCabo->codigo = codigo ;
   diagCabo->iadm_a = 100;
   diagCabo->vmax_kv = 15;
   diagCabo->vmin_kv = 0;
   diagCabo->r_ohm_km = 0.0;
   diagCabo->raio_m = 0.0;
   diagCabo->gmr_m = 0.0;
   diagCabo->preReunido = false;
   diagCabo->z0_r_ohm_km = 0.02;
   diagCabo->z0_x_ohm_km = 0.02;
   diagCabo->z1_r_ohm_km = 0.02;
   diagCabo->z1_x_ohm_km = 0.02;
   diagCabo->c0_nf_km = 0;
   diagCabo->c1_nf_km = 0;

   return diagCabo;
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCabo::Executa(void)
{  //variaveis locais
   VTArranjos   *arranjos = (VTArranjos*) apl->GetObject(__classid(VTArranjos));
   VTCabo    *cabo;

   //inicialza infos
   infoCabos.n_total = 0;
   infoCabos.n_verif = 0;
   infoCabos.n_ocorrencias =0;
   //if(log_cabo){log_cabo->Clear();}
   if(log_cabo)IniciaLog(log_cabo);
   try
   {
      infoCabos.n_total = arranjos->LisCabo()->Count;
      //percorre todos os cabos
      for(int na = 0; na < arranjos->LisCabo()->Count; na++)
      {
         cabo = (VTCabo*) arranjos->LisCabo()->Items[na];
         temErro = false;
         if(cabo){VerificaCabo(cabo);}
         if(temErro)infoCabos.n_ocorrencias++;
      }
      //altera o boolean mostrando que foi executado
      executado = true;
      return true;
   }
   catch(Exception &e)
   {return false;}
}

//---------------------------------------------------------------------------
//bool   __fastcall  TDiagCabo::ExisteCfgTipoRede(int redeSEGMENTO)
//{  //variaveis locais
//   DIAG_CABO *diagCabo;
//
//   for(int nt = 0; nt < listCfg->Count; nt++)
//   {
//      diagCabo = (DIAG_CABO*) listCfg->Items[nt];
//      if (diagCabo->tipo_rede == redeSEGMENTO) {return true;}
//   }
//   return false;
//}

//---------------------------------------------------------------------------
DIAG_CABO*  __fastcall  TDiagCabo::Get_DiagCabo(int id)
{  //variaveis locais
   DIAG_CABO *diagCabo;

   for(int i = 0 ; i < listCfg->Count; i++)
   {
      diagCabo = (DIAG_CABO*) listCfg->Items[i];
      if(id == diagCabo->id){return diagCabo;}
   }
   return NULL;
}


//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::Inicia(void)
{
   //primeiramente carrega as configs do arquivo
   LeArquivo();
   //verifica se falta configs pros tipos de rede existentes
   //VerificaTiposRede();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();

   //checkListBox->Items->Add("Comparar com catálogo");
   checkListBox->Items->Add(op_CAB_TEXT[opCabCATALOGO]);
   checkListBox->Checked[opCabCATALOGO] = this->OpcaoDiagExisteNoCatalogo;

   checkListBox->Items->Add(op_CAB_TEXT[opCabRAIO]);
   //checkListBox->Items->Add("Catálogo: Raio [m]");
   checkListBox->Checked[opCabRAIO] = this->OpcaoDiagExisteNoCatalogo;

   //checkListBox->Items->Add("Catálogo: Resistência [ohm/km]");
   checkListBox->Items->Add(op_CAB_TEXT[opCabRESIST]);
   checkListBox->Checked[opCabRESIST] = this->OpcaoDiagExisteNoCatalogo;

   checkListBox->Items->Add(op_CAB_TEXT[opCabRMG]);
   //checkListBox->Items->Add("Catálogo: RMG [m]");
   checkListBox->Checked[opCabRMG] = this->OpcaoDiagExisteNoCatalogo;
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colCab_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {
      //limpa o log
      log->Clear();

      linha[colCab_EQPTO] = "Eqpto" + Separador;
      linha[colCab_CODIGO] = "Código" + Separador;
      linha[colCab_TIPOERRO] = "Tipo Erro" + Separador;
      linha[colCab_PADRAO] = "Cabo Padrão" + Separador;
      linha[colCab_DESCRICAO] = "Descrição" + Separador;
      linha[colCab_VAL_PADRAO] = "Valor do Padrão" + Separador;
      linha[colCab_VAL_CABO] = "Valor do Cabo";
      //concatena os campos em uma linha
      for (int i = 0; i < colCab_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::PreencheListView(TListView *listView)
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
   ExtraiStrings(log_cabo->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCab_EQPTO];
   //listColumn->Caption = "Equipamento";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCab_CODIGO];
   //listColumn->Caption = "Codigo";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCab_TIPOERRO];
   //listColumn->Caption = "Tipo Erro";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCab_PADRAO];
   //listColumn->Caption = "Cabo Padrão";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 300;
   listColumn->Caption = campos->Strings[colCab_DESCRICAO];
   //listColumn->Caption = "Descrição";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 90;
   listColumn->Caption = campos->Strings[colCab_VAL_PADRAO];
   //listColumn->Caption = "Valor Padrão";
   //listColumn->Width = ColumnHeaderWidth;
   listColumn = listView->Columns->Add();
   listColumn->Width = 90;
   listColumn->Caption = campos->Strings[colCab_VAL_CABO];
   //listColumn->Caption = "Valor Cabo";
   //listColumn->Width = ColumnHeaderWidth;

   //coloca valor fixo de width
//   for(int nc =0; nc <listView->Columns->Count; nc++)
//   {
//      listView->Columns->Items[index]->Width = 50;
//   }

   for(int nl = 1; nl < log_cabo->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_cabo->Strings[nl], ";", campos);

      listItem = listView->Items->Add();
      listItem->Data = log_cabo->Objects[nl];
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
long __fastcall TDiagCabo::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoCabos.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagCabo::PM_GetLog(void)
{
   if(executado)
   {
      return log_cabo;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagCabo::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoCabos.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}
//---------------------------------------------------------------------------
bool  __fastcall  TDiagCabo::LeArquivo(void)
{  //variaveis locais
   VTInfoset   *infoset;
   DIAG_CABO   *diagCabo;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   bool        temPadrao = false;
   AnsiString  booleano = "";
   //bool        *prereunido = false;

   try {
      //cria novo infoset para ler o arquivo de cfg
      infoset = geral->Infoset;
      infoset->Clear();
      //verifica se é possivel ler o arquivo
      if(infoset->LoadFromFile(FILE_CFG_CABO))
      {  //varre todo o arquivo lido
         for (int i = 0; i < infoset->Count; i++)
         {
            infoset->SelectLine(i);
            diagCabo = new DIAG_CABO;
            infoset->GetInfo("id",           diagCabo->id          );
            infoset->GetInfo("cfg_cabo",     diagCabo->cfg_cabo    );
            infoset->GetInfo("codigo",       diagCabo->codigo      );
            infoset->GetInfo("iadm_a",       diagCabo->iadm_a      );
            infoset->GetInfo("vmax_kv",      diagCabo->vmax_kv     );
            infoset->GetInfo("vmin_kv",      diagCabo->vmin_kv     );
            infoset->GetInfo("r_ohm_km",     diagCabo->r_ohm_km    );
            infoset->GetInfo("raio_m",       diagCabo->raio_m      );
            infoset->GetInfo("gmr_m",        diagCabo->gmr_m       );
            infoset->GetInfo("prereunido",   booleano);
            diagCabo->preReunido = StringParaBool(booleano);
            //infoset->GetInfo("prereunido",   diagCabo->preReunido);
            infoset->GetInfo("z0_r_ohm_km",  diagCabo->z0_r_ohm_km );
            infoset->GetInfo("z0_x_ohm_km",  diagCabo->z0_x_ohm_km );
            infoset->GetInfo("z1_r_ohm_km",  diagCabo->z1_r_ohm_km );
            infoset->GetInfo("z1_x_ohm_km",  diagCabo->z1_x_ohm_km );
            infoset->GetInfo("c0_nf_km",     diagCabo->c0_nf_km    );
            infoset->GetInfo("c1_nf_km",     diagCabo->c1_nf_km    );
            listCfg->Add(diagCabo);
            //caso o tipo rede seja 0 (== PADRAO)
            if(diagCabo->id == 0){temPadrao = true;}
         }
      }//caso não exista o arquivo
      else
      {  //cria o tipo "default"
         diagCabo = CriaCfgCaboDefault(0, "*Padrão*");
         //adiciona à lista de configs
         listCfg->Add(diagCabo);
         temPadrao = true;
         //e cria o arquivo
         SalvaArquivo();
      }
      if (!temPadrao)
      {  //cria o tipo "default"
         diagCabo = CriaCfgCaboDefault(0, "*Padrão*");
         //define como padrao
         //diagCaboPADRAO = diagCabo;
         //adiciona à lista de configs
         listCfg->Add(diagCabo);
         temPadrao = true;
      }
   return true;
   } catch (Exception &e) {return false;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagExisteNoCatalogo = checkListBox->Checked[opCabCATALOGO];
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCabo::SalvaArquivo(void)
{  /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
   //variaveis locais
   VTInfoset   *infoset;
   DIAG_CABO   *diagCabo;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString  booleano = "";

   //infoset = NewObjInfoset(apl);
   //infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
   try{
      infoset = geral->Infoset;
      infoset->Clear();
      for (int i = 0; i < listCfg->Count; i++)
      {
         diagCabo = (DIAG_CABO*) listCfg->Items[i];
         infoset->InsertNewLine();
         infoset->AddInfo("id",          i);    /* TODO : Pensar melhor nessa id, id muda de acordo com a posicao na lista... */
         infoset->AddInfo("cfg_cabo",    diagCabo->cfg_cabo);
         infoset->AddInfo("codigo",      diagCabo->codigo);
         infoset->AddInfo("iadm_a",      diagCabo->iadm_a);
         infoset->AddInfo("vmax_kv",     diagCabo->vmax_kv);
         infoset->AddInfo("vmin_kv",     diagCabo->vmin_kv);
         infoset->AddInfo("r_ohm_km",    diagCabo->r_ohm_km);
         infoset->AddInfo("raio_m",      diagCabo->raio_m);
         infoset->AddInfo("gmr_m",       diagCabo->gmr_m);
         booleano = BooleanoParaString(diagCabo->preReunido);
         infoset->AddInfo("prereunido",  booleano);
         //infoset->AddInfo("prereunido",  diagCabo->preReunido);
         infoset->AddInfo("z0_r_ohm_km", diagCabo->z0_r_ohm_km);
         infoset->AddInfo("z0_x_ohm_km", diagCabo->z0_x_ohm_km);
         infoset->AddInfo("z1_r_ohm_km", diagCabo->z1_r_ohm_km);
         infoset->AddInfo("z1_x_ohm_km", diagCabo->z1_x_ohm_km);
         infoset->AddInfo("c0_nf_km",    diagCabo->c0_nf_km);
         infoset->AddInfo("c1_nf_km",    diagCabo->c1_nf_km);
      }
      infoset->SaveToFile(FILE_CFG_CABO);
      return true;
   }
   catch (Exception &e)
   {
      return false;
   }
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::ShowFormCfg(void)
{
   if(formCatalogoCab) formCatalogoCab->Show();
   else
   {
      formCatalogoCab = new TFormCatalogoCab(NULL, apl);
      formCatalogoCab->Inicia(this);
      formCatalogoCab->Show();
   }
}

//---------------------------------------------------------------------------
int   __fastcall  TDiagCabo::ShowFormCfgModal(void)
{
//   if(formDiagEqptoCabo)
//   {
//    return formDiagEqptoCabo->ShowModal();
//   }
//   else
//   {
//      formDiagEqptoCabo = new TFormDiagEqptoCabo(NULL, apl);
//      formDiagEqptoCabo->Inicia(this);
//      return formDiagEqptoCabo->ShowModal();
//   }
//   return -1;    /* TODO : pensar melhor */
   return false;
}
//---------------------------------------------------------------------------
int   __fastcall  TDiagCabo::Tipo(void)
{
   return(dgeCABO);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagCabo::TipoAsString(void)
{
   return "Cabos";
}

//---------------------------------------------------------------------------
//void   __fastcall  TDiagCabo::VerificaTiposRede(void)
//{
//   VTTipos    *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   TList      *lisTIPOREDE = tipos->LisTipoRede();
//   VTTipoRede *tipo_rede;
//   DIAG_CABO *diagCabo;
//
//   //verifica os tipos de rede cadastrados na base
//   //em busca de dos tipos de rede que não possuem cfg no arquivo lido
//   for (int i = 0; i < lisTIPOREDE->Count; i++)
//   {
//      tipo_rede  = (VTTipoRede*) lisTIPOREDE->Items[i];
//      if( !ExisteCfgTipoRede(tipo_rede->Segmento) )
//      {  //quando encontra um tipo sem correspondente, cria um "default"
//         diagCabo = CriaCfgCaboDefault(tipo_rede->Segmento, tipo_rede->Codigo);
//         //adiciona à lista de configs
//         listCfg->Add(diagCabo);
//      }
//      else
//      {
//         //faz nada
//      }
//   }
//}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::VerificaCabo(VTCabo *cabo)
{  //variaveis locais
   DIAG_CABO *diagCabo;

   //verifica se existe no catalogo
   diagCabo = VerificaOpExisteNoCatalogo(cabo);
   //caso encontre no catalogo, pode-se fazer outras verificações
   if(diagCabo)
   {  //verifica se os valores são compativeis com o cabo padrao encontrado
      if (dgCabo.raio)VerificaOpRaio(cabo, diagCabo);
      //verifica se os valores são compativeis com o cabo padrao encontrado
      if (dgCabo.resist)VerificaOpResist(cabo, diagCabo);
      //verifica se os valores são compativeis com o cabo padrao encontrado
      if (dgCabo.rmg)VerificaOpRmg(cabo, diagCabo);
      //incrementa o contador de verificacoes
      infoCabos.n_verif++;
   }
}

//---------------------------------------------------------------------------
//Provavel problema de desempenho!
DIAG_CABO*  __fastcall  TDiagCabo::VerificaOpExisteNoCatalogo(VTCabo *cabo)
{  //variaveis locais
   DIAG_CABO *diagCabo;
   bool      encontrou = false;

   for(int ni = 0; ni < listCfg->Count;ni++)
   {

      diagCabo =  (DIAG_CABO*)listCfg->Items[ni];
      if(diagCabo->codigo.AnsiCompareIC(cabo->Codigo) == 0)
      {  //correspondencia exata
         encontrou = true;
         return diagCabo;
      }
      else
      {
         if(diagCabo->codigo.AnsiCompareIC(cabo->Codigo) < 0)
         {
            if(cabo->Codigo.Pos(diagCabo->codigo) > 0)
            {//contem a string
               encontrou = true;
               return diagCabo;
            }
         }
         else
         {
            if(diagCabo->codigo.Pos(cabo->Codigo) > 0)
            {//contem a string
               encontrou = true;
               return diagCabo;
            }
         }
      }
   }
   //se não encontrou
   if(!encontrou)
   {
      if (dgCabo.existeNoCatalogo)AddLinhaLog(cabo, opCabCATALOGO);
   }
   return NULL;
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::VerificaOpRaio(VTCabo *cabo, DIAG_CABO *diagCabo)
{  //variaveis locais
   double a, b;

   //verifica os valores do raio [m]
   a = cabo->raio;
   b = diagCabo->raio_m;
   if(!IsDoubleZero(a - b,1e-3))
   {
      AddLinhaLog(cabo, opCabRAIO,diagCabo);
   }

}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::VerificaOpResist(VTCabo *cabo, DIAG_CABO *diagCabo)
{  //variaveis locais
   double a, b;

   //verifica os valores de R [ohm/km]
   a = cabo->r;
   b = diagCabo->r_ohm_km;
   if(!IsDoubleZero(a - b,1e-3))
   {
      AddLinhaLog(cabo, opCabRESIST,diagCabo);
   }
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCabo::VerificaOpRmg(VTCabo *cabo, DIAG_CABO *diagCabo)
{  //variaveis locais
   double a, b;

   //verifica os valores de R [ohm/km]
   a = cabo->gmr;
   b = diagCabo->gmr_m;
   if(!IsDoubleZero(a - b,1e-3))
   {
      AddLinhaLog(cabo, opCabRMG,diagCabo);
   }
}

//---------------------------------------------------------------------------
