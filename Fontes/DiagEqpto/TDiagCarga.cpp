//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagCarga.h"
#include "TFormDiagCarga.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//define do nome do arquivo
#define FILE_CFG_CARGA "CfgDiagEqptoCarga"
//---------------------------------------------------------------------------
//VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
//   {
//   return (new TDiagEqptoCarga(apl));
//   }
//---------------------------------------------------------------------------
__fastcall  TDiagCarga::TDiagCarga(VTApl *apl)
{
   this->apl = apl;
   //define se tem informações adicionais
   infoExtra = false;
   //cria a lista de configuracoes
   listCfg = new TList;
   //inicializa as opcoes do diagnostico
   dgCarga.energiaNula = true;
   dgCarga.consElevado = true;
   dgCarga.fases       = true;
//   dgCarga.isolado     = true;
   //cria o log_carga
                        /* TODO : mudar para o executa? */
   log_carga = new TStringList;
   //Mostra que ainda não foi executado
   executado = false;

}
//---------------------------------------------------------------------------
__fastcall  TDiagCarga::~TDiagCarga(void)
{
   //destroi lista e conteudo
   if(listCfg){ LimpaTList(listCfg); delete listCfg; listCfg = NULL;}
   //destroi log
   if(log_carga){delete log_carga; log_carga = NULL;}
   //destroi form
   if(formDiagCarga){delete formDiagCarga; formDiagCarga = NULL;}
}

//---------------------------------------------------------------------------
void         __fastcall  TDiagCarga::AddLinhaLog(VTCarga *carga,int opDIAG_CARGA, DIAG_CARGA *diagCarga)
{  //variaveis locais
   VTClasse    *classe;
   VTFases     *fases =  (VTFases*) apl->GetObject(__classid(VTFases));
   VTDemanda   *demanda =  (VTDemanda*) apl->GetObject(__classid(VTDemanda));
   double      energiaMensal = 0.;

   energiaMensal = carga->Energia_mwhmes;
   //se esse valor estiver zerado, confere com o demanda
   if((IsDoubleZero(energiaMensal)) && (demanda != NULL))
      {
      energiaMensal = demanda->Emes_mwh[carga];
      }
   //incrementa o contador de erros
   //infoCargas.n_ocorrencias++;
   temErro = true;
   //vetor de strings seguindo o enum col_LOG_CARGA
   AnsiString  linhaPreenchida = "";
   AnsiString  linha[colCar_COUNT];
   AnsiString  Separador = ";";

   if(carga) classe = carga->classe;
   //inicializa com os valores vindos do carga
   linha[colCar_EQPTO]     = "Carga" + Separador;
   linha[colCar_CODIGO]    = carga->Codigo + Separador;
   linha[colCar_CODBARRA]  = carga->pbarra->Codigo + Separador;
   //novos
   linha[colCar_FASE]  = " - " + Separador;
   linha[colCar_CONSUMO]  = " - ";
   if(classe)  linha[colCar_CLASSE] = classe->Codigo + Separador;
   else        linha[colCar_CLASSE] = " - " + Separador;
   if (carga)
   {
      switch (opDIAG_CARGA)
      {
         case opCarENERGIANULA:
            linha[colCar_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colCar_DESCRICAO] = "Carga com consumo mensal nulo." + Separador;
            //linha[colCar_VALOR]     = DoubleToStr("%4.3f", carga->Energia_mwhmes) +" [MWh/mês]";
            linha[colCar_CONSUMO]     = DoubleToStr("%4.3f", carga->Energia_mwhmes);
            break;

         case opCarCONSELEVADO:
            linha[colCar_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
            linha[colCar_DESCRICAO] = "Carga com consumo mensal elevado." + Separador;
            //linha[colCar_VALOR]     = DoubleToStr("%4.3f", carga->Energia_mwhmes) +" [MWh/mês]";
            linha[colCar_CONSUMO]   = DoubleToStr("%4.3f", energiaMensal);
            break;

         case opCarFASES:
            linha[colCar_TIPOERRO]  = str_TIPOERRO_ERRO + Separador;
            linha[colCar_DESCRICAO] = "Carga com fases incoerentes com a rede." + Separador;
            //linha[colCar_VALOR]     = fases->AsStrABCN[carga->Fases];
            linha[colCar_FASE]     = fases->AsStrABCN[carga->Fases] + Separador;
            break;

//		 case opCarISO:
//			linha[colCar_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
//			linha[colCar_DESCRICAO] = "Carga isolada." + Separador;
//			//linha[colCar_VALOR]     = " - ";
//			break;

         default:
            linha[colCar_TIPOERRO]  = " - " + Separador;
            linha[colCar_DESCRICAO] = " - " + Separador;
            //linha[colCar_VALOR]     = " - ";
            ;
      }
      //concatena os campos em uma linha
      for (int i = 0; i < colCar_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log_carga->AddObject(linhaPreenchida, carga);
   }
}

//---------------------------------------------------------------------------
DIAG_CARGA* __fastcall  TDiagCarga::CriaCfgCargaDefault(int classe, AnsiString nomeClasse)
{  //variaveis locais
   DIAG_CARGA *diagCarga;

   diagCarga = new DIAG_CARGA;
   diagCarga->classe = classe;
   diagCarga->codigo = nomeClasse;
   //poe valores default
   diagCarga->energiaMax_mwh = 5;

   return diagCarga;
}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagCarga::Executa(void)
{  //variaveis locais
   VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTCarga *carga;
   TList       *listCar = new TList();

   //inicialza infos
   infoCargas.n_total = 0;
   infoCargas.n_verif = 0;
   infoCargas.n_ocorrencias = 0;
   //if(log_carga){log_carga->Clear();}
   if(log_carga)IniciaLog(log_carga);
   try
   {
      redes->LisEqbar(listCar,eqptoCARGA);
      infoCargas.n_total = listCar->Count;
      //um cast na ligacao
      for(int nc = 0; nc < listCar->Count; nc++)
      {
         carga = (VTCarga*) listCar->Items[nc];
         //verifica o carga
         temErro = false;
         VerificaCarga(carga);
         if(temErro)infoCargas.n_ocorrencias++;
      }
      //altera o boolean mostrando que foi executado
      executado = true;
      //deleta a lista de cargaes
      if(listCar){delete listCar; listCar = NULL;}
      return true;
   }
   catch(Exception &e)
   {
      //deleta a lista de cargaes
      if(listCar){delete listCar; listCar = NULL;}
      return false;
   }
}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagCarga::ExisteCfgTipoCarga(int classe)
{  //variaveis locais
   DIAG_CARGA *diagCarga;

   for(int nt = 0; nt < listCfg->Count; nt++)
   {
      diagCarga = (DIAG_CARGA*) listCfg->Items[nt];
      if (diagCarga->classe == classe) {return true;}
   }
   return false;
}


//---------------------------------------------------------------------------
DIAG_CARGA*   __fastcall  TDiagCarga::Get_DiagCarga(int classe)
{  //variaveis locais
   DIAG_CARGA *diagCarga;

   for(int i = 0 ; i < listCfg->Count; i++)
   {
      diagCarga = (DIAG_CARGA*) listCfg->Items[i];
      if(classe == diagCarga->classe){return diagCarga;}
   }
   return NULL;
}


//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::Inicia(void)
{
   //primeiramente carrega as configs do arquivo
   LeArquivo();
   //verifica se falta configs pros tipos de rede existentes
   VerificaTiposClasse();
   //
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::IniciaCheckList(TCheckListBox *checkListBox)
{
   checkListBox->Clear();

   checkListBox->Items->Add(op_CAR_TEXT[opCarENERGIANULA]);
   checkListBox->Checked[opCarENERGIANULA] = this->OpcaoDiagEnergiaNula;

   checkListBox->Items->Add(op_CAR_TEXT[opCarCONSELEVADO]);
   checkListBox->Checked[opCarCONSELEVADO] = this->OpcaoDiagConsElevado;

   checkListBox->Items->Add(op_CAR_TEXT[opCarFASES]);
   checkListBox->Checked[opCarFASES] = this->OpcaoDiagFases;

//   checkListBox->Items->Add(op_CAR_TEXT[opCarISO]);
//   checkListBox->Checked[opCarISO] = this->OpcaoDiagIsolado;
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::IniciaLog(TStringList *log)
{  //variaveis locais
   AnsiString  Separador = ";";
   AnsiString  linha[colCar_COUNT];
   AnsiString  linhaPreenchida = "";
   if(log)
   {
      //limpa o log
      log->Clear();

      //linha[colTch_SEGMENTO] = "Segmento" + Separador;
      //linha[colTch_REDE] = "Rede" + Separador;
      linha[colCar_EQPTO] = "Eqpto" + Separador;
      linha[colCar_CODIGO] = "Código" + Separador;
      linha[colCar_TIPOERRO] = "Tipo Erro" + Separador;
      linha[colCar_CODBARRA] = "Barra" + Separador;
      linha[colCar_DESCRICAO] = "Descrição" + Separador;
      linha[colCar_CLASSE] = "Classe" + Separador;
      linha[colCar_FASE] = "Fase" + Separador;
      linha[colCar_CONSUMO] = "Consumo [MWh/mês]" ;
      //linha[colCar_VALOR] = "Valor";
      //concatena os campos em uma linha
      for (int i = 0; i < colCar_COUNT; i++)
      {
         linhaPreenchida += linha[i];
      }
      log->Add(linhaPreenchida);
   }
}
//---------------------------------------------------------------------------
long __fastcall  TDiagCarga::PM_GetItensVerificados(void)
{
   if(executado)
   {
      return infoCargas.n_verif;
   }
   return -1;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TDiagCarga::PM_GetLog(void)
{
   if(executado)
   {
      return log_carga;
   }
   else
   {
      return NULL;
   }
}
//---------------------------------------------------------------------------
long  __fastcall  TDiagCarga::PM_GetOcorrencias(void)
{
   if(executado)
   {
      return infoCargas.n_ocorrencias;
   }
   else
   {
      return -1;
   }
}
//---------------------------------------------------------------------------
void     __fastcall  TDiagCarga::PreencheListView(TListView *listView)
{  //variaveis locais
   TListItem   *listItem;
   TListColumn *listColumn;
   TStringList *campos = new TStringList();

   //limpa o listView
   listView->Clear();
   listView->Columns->Clear();

   campos->Clear();
   //Extrai campos da linha
   ExtraiStrings(log_carga->Strings[0], ";", campos);

   //adiciona as colunas
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCar_EQPTO];
   //listColumn->Cartion = "Eqpto";
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCar_CODIGO];
   //listColumn->Cartion = "Código";
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCar_TIPOERRO];
  // listColumn->Cartion = "Tipo Erro";
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCar_CODBARRA];
   //listColumn->Cartion = "Barra";
   listColumn = listView->Columns->Add();
   listColumn->Width = 250;
   listColumn->Caption = campos->Strings[colCar_DESCRICAO];
   listColumn = listView->Columns->Add();
   listColumn->Width = 70;
   listColumn->Caption = campos->Strings[colCar_CLASSE];
//   listColumn = listView->Columns->Add();
//   listColumn->Width = 70;
//   listColumn->Caption = campos->Strings[colCar_VALOR];
   listColumn = listView->Columns->Add();
   listColumn->Width = 40;
   listColumn->Caption = campos->Strings[colCar_FASE];
   listColumn = listView->Columns->Add();
   listColumn->Width = 90;
   listColumn->Caption = campos->Strings[colCar_CONSUMO];
   //listColumn->Cartion = "Descrição";

   for(int nl = 1; nl < log_carga->Count; nl++)
   {
      campos->Clear();
      //Extrai campos da linha
      ExtraiStrings(log_carga->Strings[nl], ";", campos);
      listItem = listView->Items->Add();
      listItem->Data = log_carga->Objects[nl];
      listItem->Caption = campos->Strings[0] ;
      for(int nc = 1; nc < campos->Count; nc++)
      {
         listItem->SubItems->Add(campos->Strings[nc]);
      }
   }
   //destroi o TStrings
   if(campos){delete campos; campos = NULL;}
}

//---------------------------------------------------------------------------
bool     __fastcall  TDiagCarga::LeArquivo(void)
{  //variaveis locais
   VTInfoset   *infoset;
   DIAG_CARGA *diagCarga;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   bool        temPadrao = false;

   try {
      //cria novo infoset para ler o arquivo de cfg
      infoset = geral->Infoset;
      infoset->Clear();
      //verifica se é possivel ler o arquivo
      if(infoset->LoadFromFile(FILE_CFG_CARGA))
      {  //varre todo o arquivo lido
         for (int i = 0; i < infoset->Count; i++)
         {
            infoset->SelectLine(i);
            diagCarga = new DIAG_CARGA;
            infoset->GetInfo("classe", diagCarga->classe);
            infoset->GetInfo("codigo", diagCarga->codigo);
            infoset->GetInfo("energiaMax_mwh", diagCarga->energiaMax_mwh);
            listCfg->Add(diagCarga);
            //caso o tipo rede seja 0 (== PADRAO)
            if(diagCarga->classe == 99){temPadrao = true;}
         }
      }
      //caso não exista o arquivo
      else
      {  //cria o tipo "default"
         diagCarga = CriaCfgCargaDefault(99, "*Padrão*");
         //define como padrao
         //adiciona à lista de configs
         listCfg->Add(diagCarga);
         temPadrao = true;
         //e cria o arquivo
         SalvaArquivo();
      }
      if (!temPadrao)
      {  //cria o tipo "default"
         diagCarga = CriaCfgCargaDefault(99, "*Padrão*");
         //adiciona à lista de configs
         listCfg->Add(diagCarga);
         temPadrao = true;
      }
   return true;
   } catch (Exception &e) {return false;}
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::LeCheckList(TCheckListBox *checkListBox)
{
   this->OpcaoDiagEnergiaNula = checkListBox->Checked[opCarENERGIANULA];
   this->OpcaoDiagConsElevado = checkListBox->Checked[opCarCONSELEVADO];
   this->OpcaoDiagFases       = checkListBox->Checked[opCarFASES];
//   this->OpcaoDiagIsolado     = checkListBox->Checked[opCarISO];
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCarga::SalvaArquivo(void)
{  /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
   //variaveis locais
   VTInfoset   *infoset;
   DIAG_CARGA  *diagCarga;
   VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   try{
      infoset = geral->Infoset;
      infoset->Clear();
      for (int i = 0; i < listCfg->Count; i++)
      {
         diagCarga = (DIAG_CARGA*) listCfg->Items[i];
         infoset->InsertNewLine();
         infoset->AddInfo("classe", diagCarga->classe);
         infoset->AddInfo("codigo", diagCarga->codigo);
         infoset->AddInfo("energiaMax_mwh", diagCarga->energiaMax_mwh);
      }
      infoset->SaveToFile(FILE_CFG_CARGA);
      return true;
   }
   catch (Exception &e){return false;}
}
//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::ShowFormCfg(void)
{
   if(formDiagCarga) formDiagCarga->Show();
   else
   {
      formDiagCarga = new TFormDiagCarga(NULL, apl);
      formDiagCarga->Inicia(this);
      formDiagCarga->Show();
   }
}

//---------------------------------------------------------------------------
int   __fastcall  TDiagCarga::ShowFormCfgModal(void)
{
   if(!formDiagCarga)
   {
      formDiagCarga = new TFormDiagCarga(NULL, apl);
      formDiagCarga->Inicia(this);
   }

   return formDiagCarga->ShowModal();
}
//---------------------------------------------------------------------------
int   __fastcall  TDiagCarga::Tipo(void)
{
   return(dgeCARGA);
}
//---------------------------------------------------------------------------
AnsiString  __fastcall  TDiagCarga::TipoAsString(void)
{
   return "Cargas";
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::VerificaTiposClasse(void)
{
   VTClasses   *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
   TList       *lisCLASSES = classes->LisClasse();
   VTClasse    *classe;
   DIAG_CARGA  *diagCarga;

   //verifica as classes cadastradas na base
   //em busca de classes que não possuem cfg no arquivo lido
   for (int i = 0; i < lisCLASSES->Count; i++)
   {
      classe  = (VTClasse*) lisCLASSES->Items[i];
      if( !ExisteCfgTipoCarga(classe->Id) )
      {  //quando encontra um tipo sem correspondente, cria um "default"
         diagCarga = CriaCfgCargaDefault(classe->Id, classe->Codigo);
         //adiciona à lista de configs
         listCfg->Add(diagCarga);
      }
      else
      {
         //faz nada
      }
   }
}

//---------------------------------------------------------------------------
void  __fastcall  TDiagCarga::VerificaCarga(VTCarga *carga)
{  //variaveis locais
   int         id_classe;
   VTClasse    *classe;
   DIAG_CARGA  *diagCarga;

   //incrementa o numero de verificações
   infoCargas.n_verif++;
    //verifica que tipo de chave
   classe = carga->classe;
   //caso o trecho não tenha tipo associada, a verificação sera com o Padrao
   if (!classe){id_classe = 99;}
   //caso exista, associa ao tipo de rede da rede associada ao trecho
   else{id_classe = classe->Id;}
   if((diagCarga = Get_DiagCarga(id_classe)) != NULL)
   {
      if(dgCarga.consElevado)VerificaOpConsumoElevado(carga,diagCarga);
   }
   if(dgCarga.energiaNula)VerificaOpEnergiaNula(carga);
   if(dgCarga.fases)VerificaOpFases(carga);
//   if(dgCarga.isolado)VerificaOpIsolado(carga);
}

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCarga::VerificaOpConsumoElevado(VTCarga *carga, DIAG_CARGA *diagCarga)
{  //variaveis locais
   bool        sucesso = true;
   VTDemanda   *demanda =  (VTDemanda*) apl->GetObject(__classid(VTDemanda));
   double      energiaMensal = 0.;

   energiaMensal = carga->Energia_mwhmes;
   //se esse valor estiver zerado, confere com o demanda
   if((IsDoubleZero(energiaMensal)) && (demanda != NULL))
      {
      energiaMensal = demanda->Emes_mwh[carga];
      }

   if(energiaMensal > diagCarga->energiaMax_mwh)
   {
      AddLinhaLog(carga, opCarCONSELEVADO);
      sucesso = false;
   }
   return sucesso;
}
//---------------------------------------------------------------------------
bool  __fastcall  TDiagCarga::VerificaOpEnergiaNula(VTCarga *carga)
{  //variaveis locais
   VTCurva  *curva;
   //double   valorMédio;
   bool     valoresMédiosZero[nvCURVA_RRSUP];
   bool     andValor;
   bool     sucesso = true;


   //se for carga ip
   if(carga->IP)
      {
      if(carga->IP_NumLamp == 0)
         {
         AddLinhaLog(carga, opCarENERGIANULA);
         sucesso = false;
         }
      else if(IsDoubleZero(carga->IP_Pot_kw))
         {
         AddLinhaLog(carga, opCarENERGIANULA);
         sucesso = false;
         }
      }
   else
      {
      //inicizaliza o vetor de bool
      for (int i = 0; i < nvCURVA_RRSUP; i++)
         {
         valoresMédiosZero[i] = true;
         }
      //guarda a curva da carga
      curva = carga->curva;
      if(curva)
         {
         if(curva->Tipica)
            {  //tiver energia zerada
            if(IsDoubleZero(carga->Energia_kwhmes))
               {
               AddLinhaLog(carga, opCarENERGIANULA);
               sucesso = false;
               }
            }
         else
            {  //se tiver curva propria
            for(int nv = 0; nv < curva->NumeroValores; nv++)
               {
               valoresMédiosZero[nv] = (IsDoubleZero(curva->Media(nv)) && IsDoubleZero(curva->Minimo(nv)));
               if(nv == 0)andValor = valoresMédiosZero[nv];
               else
                  {
                  andValor = (andValor && valoresMédiosZero[nv]);
                  }
               }
            if(andValor)
               {
               AddLinhaLog(carga, opCarENERGIANULA);
               sucesso = false;
               }
            }
         }
      else
         {
         //se não tiver curva e tiver energia zerada
         if(IsDoubleZero(carga->Energia_kwhmes))
            {
            AddLinhaLog(carga, opCarENERGIANULA);
            sucesso = false;
            }
         }
      }
  return sucesso;
   }

//---------------------------------------------------------------------------
bool  __fastcall  TDiagCarga::VerificaOpFases(VTCarga *carga)
{  //variaveis locais
   VTLigacao *ligacao;
   //VTFases  *fases =  (VTFases*) apl->GetObject(__classid(VTFases));
   VTRedes  *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList    *lisLigTemp = new TList; //ligacoes ligadas aquela barra
   int      fases_tag = 0x00; //fase vazia
   bool     sucesso = true;

   //preenche a lista com as ligações
   redes->LisLigacao(lisLigTemp, carga->pbarra);
   //percorre cada ligação pra ver quais fases estão disponíveis na barra
   for(int nl =0; nl < lisLigTemp->Count; nl++)
   {
      ligacao = (VTLigacao*)lisLigTemp->Items[nl];
      //adiciona a fase à barra
      fases_tag |= ligacao->Fases(carga->pbarra);
   }
   //adiciona o terra
   fases_tag |= faseT;
   //verifica se todas as fases da carga estão disponiveis
   if((fases_tag & carga->Fases) != carga->Fases)
   {
      AddLinhaLog(carga, opCarFASES);
      sucesso = false;
   }
   //delete a lista
   if(lisLigTemp){delete lisLigTemp; lisLigTemp = NULL;}
   return sucesso;
}
//---------------------------------------------------------------------------
//bool  __fastcall  TDiagCarga::VerificaOpIsolado(VTCarga *carga)
//{  //variaveis locais
//   bool     sucesso = true;
//   VTBarra  *barra;
//
//   barra = carga->pbarra;
//   if(barra->zona == NULL)
//   {
//	  AddLinhaLog(carga, opCarISO);
//      sucesso = false;
//   }
//
//   return sucesso;
//}
//---------------------------------------------------------------------------
