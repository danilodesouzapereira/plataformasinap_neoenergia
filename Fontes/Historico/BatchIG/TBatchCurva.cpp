//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBatchCurva.h"
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDgeral.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\CfgPerdaTec\VTCfgPerdaTec.h>
#include <Fontes\ImportaMedicao\VTImportaMedicao.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\InterfaceTXT\Importa\VTImportaTXT.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTListeqp.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Redutor\VTRedutor.h>
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\TArquivos.h"
#include <DLL_Inc\BD.h>
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\Edita.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaMedicao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\InterfaceTXT.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Redutor.h>

//---------------------------------------------------------------------------
VTBatchIG* __fastcall NewObjBatchCurva(VTApl *apl_owner)
   {
   return(new TBatchCurva(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TBatchCurva::TBatchCurva(VTApl *apl_owner)
   {//classe criada para trocar curva tipica da empresa pela de Rondonia
   //cria objeto Apl próprio
//   apl               = NewObjApl(NULL, apl_owner);
	apl = apl_owner; //não cria apl próprio, mas usa o do TFormBatchIG
	apl->Add(radial   = DLL_NewObjRadial(apl));
   apl->Add(medicoes = DLL_NewObjMedicoes(apl));
	apl->Add(salva    = DLL_NewObjSalva(NULL, apl));
   //cria demais objetos
   files  = new TStringList();
	lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBatchCurva::~TBatchCurva(void)
   {
   //destrói objetos
//   if (apl)   {delete apl;   apl   = NULL;}
	if (files) {delete files; files = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TBatchCurva::CodigoRedeSupridora(void)
   {
   //variáveis locais
   TList        *lisEXT;
   VTSubestacao *subestacao;

   //verifica se há Subestacao
   lisEXT = radial->LisSubestacao();
   if (lisEXT->Count > 0)
      {
      subestacao = (VTSubestacao*)lisEXT->First();
      return(subestacao->Rede->Codigo);
      }
   return("Indefinida");
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::CriaBaseAccessDaRede(AnsiString arq_rede)
   {
   //variáveis locais
   VTDir  *dir   = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath *path  = (VTPath*)apl->GetObject(__classid(VTPath));

   //cria uma nova base de dados Access
   if (! dir->CriaCopiaRedeSinap(arq_rede, false)) return(false);
   //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir já aponta a nova base)
   path->ArqRede = arq_rede;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::ExcluiCurvas(void)
   {
   //var locais
   VTCurva  *curva;
   VTCurvas *curvas    = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   TList    *lisCURVA;

   curvas->DiaMedicao = SEMANA_COMPLETA;
   //exclui todas curvas tipicas de carga
   if(! curvas) return(false);
   lisCURVA = curvas->ListaCurvaTipicaCarga();
   for(int nc = 0; nc < lisCURVA->Count; nc++)
      {
      curva = (VTCurva*)lisCURVA->Items[nc];
      curva->Status[sttEXCLUIDO] = true;
      SalvaBaseCurvaTipica(curva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::ExisteCurva(TList *lisCURVA, VTCurva *curva)
   {
   return(lisCURVA->IndexOf(curva) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::Executa(TListBox *LBoxLog)
   {
   //variáveis locais
   bool        sucesso = true;
   AnsiString  filename;
   TDateTime   data;
   TArqRede    *arq_rede;
   TArquivos   *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));
   VTRedes     *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//salva TStrings usado para apresentar mensagens indicando o progresso
      this->LBoxLog = LBoxLog;
      //calcula perda técnica para todos meses do período
      for (data = DataIni; data <= DataFim; data = IncMonth(data, 1))
         {//reinicia Redes
         redes->Clear();
         //atualiza progresso
         Progresso(data);
         //determina ArqRede da data indicada
         if ((arq_rede = arquivos->ExisteArqRede(data)) == NULL) return(false);
         //loop para todas as redes definidas em ArqRede
         for (int n = 0; n < arq_rede->files->Count; n++)
            {//lê as rede do arquivo
            filename = arq_rede->files->Strings[n];
            Progresso(AnsiString(ExtractFileName(filename)));
            switch(arquivos->TipoArqRede)
               {
               case arqSINAP:     sucesso = LeRedesBaseSinap(filename);              break;
               default: return(false);
//               case arqGIS:       sucesso = ImportaRedesArqGIS(data, filename);      break;
//               case arqLIS_GIS:   sucesso = ImportaRedesListaArqGIS(data, filename); break;
               }
            if (! sucesso) continue;
            //exclui todas as curvas
            sucesso = ExcluiCurvas();
            if (! sucesso) continue;
            //importa curvas do TXT modelo Sinap
            sucesso = ImportaCurvas();
            if (! sucesso) continue;
            //salva rede na base de dados
            if (! SalvaRedeNaBase(data)) continue;
            }
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::ImportaCurvas(void)
   {
   //var locais
   AnsiString   arq_txt;
   bool         sucesso;
   VTImportaTXT *importa;
   VTPath       *path  = (VTPath*)apl->GetObject(__classid(VTPath));

   try{//cria objeto importa txt
      importa = DLL_NewObjImportaTXT(apl);
      //importa arquivos txt
      arq_txt = path->DirGeral() + "\\ArqCfgEmpresa_Rondonia3.txt";
      //arquivo de dados de um primário
      sucesso = importa->ImportaCurvasTipicas(arq_txt);
      //destrói objeto importa
      delete importa;
      importa = NULL;
      }catch(Exception &e)
         {
         if(importa) delete importa;
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
   {
   //variáveis locais
   bool              sucesso;
	strOPImporta      opcao;
   VTImportaRedeGIS *ImportaRedeGIS;

   //verifica se existe o arquivo
   if (! FileExists(filename)) return(false);
   //define opções p/ importação
//   opcao.date               = data;
//   opcao.sub.topologia      = opSUB_GIS;
//   opcao.pri.consA4         = opENERGIA;
//   opcao.fator_carga        = 0.6;
//   opcao.sec.topologia      = opSEC_GIS;//opSEC_CARGA;
//   opcao.valida_fase        = true;
//   opcao.corrige_fase       = true;
//   opcao.corrige_perda_fe   = true;
//   opcao.valida_eqpto       = true;
//   opcao.gerarTXT           = false;
//   opcao.ihm.log            = false;
//   opcao.ihm.progresso      = true;
	opcao = PD.opcao;
	//cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL) return(false);
   //inicia files com o arquivo com lista de arquivos
   files->Clear();
   files->Add(filename);
   //executa importação da rede
   sucesso = ImportaRedeGIS->ImportaLisArqPrimario(files, &opcao);
   //destrói objeto Conversor
   delete ImportaRedeGIS;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::ImportaRedesListaArqGIS(TDateTime data, AnsiString filename)
   {
   //variáveis locais
   bool              sucesso;
   strOPImporta      opcao;
   VTImportaRedeGIS *ImportaRedeGIS;

   //verifica se existe o arquivo
   if (! FileExists(filename)) return(false);
   //define opções p/ importação
//   opcao.date               = data;
//   opcao.sub.topologia      = opSUB_GIS;
//   opcao.pri.consA4         = opENERGIA;
//   opcao.fator_carga        = 0.6;
//   opcao.sec.topologia      = opSEC_GIS;//opSEC_CARGA;
//   opcao.valida_fase        = true;
//   opcao.corrige_fase       = true;
//   opcao.corrige_perda_fe   = true;
//   opcao.valida_eqpto       = true;
//   opcao.gerarTXT           = false;
//   opcao.ihm.log            = false;
//   opcao.ihm.progresso      = true;
	opcao = PD.opcao;
	//cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL) return(false);
   //inicia files com o arquivo com lista de arquivos
   files->Clear();
   files->Add(filename);
   //executa importação da rede
   sucesso = ImportaRedeGIS->ImportaLisArqSubestacao(files, &opcao);
   //destrói objeto Conversor
   delete ImportaRedeGIS;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::LeRedesBaseSinap(AnsiString filename)
   {
   //variáveis locais
   bool    sucesso;
   VTAbre  *abre;
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
//   VTDir   *dir  = (VTDir*)apl->GetObject(__classid(VTDir));

   try{//atualiza progresso
      Progresso(AnsiString("Lendo rede da base"));
      //atualiza arquivo da rede em VTPath
      path->ArqRede = filename;
      //atualiza Dir com o arquivo da rede aberta
//      dir->ArqRede = path->ArqRede;
      //cria objeto VTAbre
      abre = DLL_NewObjAbre(NULL, apl);
      //desabilita progresso
      abre->Progresso    = true;
      abre->AvisoNumRede = 0;
      //lê a rede completa
      sucesso = abre->CarregaRedeCompleta();
      //destrói objeto VTAbre
      delete abre;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TBatchCurva::Progresso(AnsiString txt)
   {
   if (LBoxLog)
      {
      LBoxLog->Items->Add(txt);
      LBoxLog->Refresh();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBatchCurva::Progresso(TDateTime data)
   {
   //variáveis locais
   AnsiString txt;

   if (LBoxLog)
      {
      LBoxLog->Clear();
      LBoxLog->Items->Add(txt.sprintf("%02d/%4d", MonthOf(data), YearOf(data)));
      LBoxLog->Refresh();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBatchCurva::RedefineId(void)
   { //DVK 2016.04.12
   //variáveis locais
   VTRedes     *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArranjos  *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTRede      *rede;
   TList       *lisREDE  = redes->LisRede();
   TList       *lisTMP   = new TList();

   //redefine IDs de todos Eqptos
   RedefineId(arranjos->LisSuporte());
   RedefineId(arranjos->LisCabo());
   RedefineId(arranjos->LisArranjo());
   RedefineId(redes->LisDominio());
   RedefineId(redes->LisRede());
   RedefineId(redes->LisCorte());
   RedefineId(redes->LisBarra());
   lisTMP->Clear(); redes->LisEqbar(lisTMP);   RedefineId(lisTMP);
   RedefineIdCurvaPropria(lisTMP);
   lisTMP->Clear(); redes->LisLigacao(lisTMP); RedefineId(lisTMP);
   lisTMP->Clear(); redes->LisReducao(lisTMP); RedefineId(lisTMP);
   lisTMP->Clear(); redes->LisMutua(lisTMP);   RedefineId(lisTMP);
   lisTMP->Clear();
   //reinicia id das Barras inicias das redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->DefineBarraInicial(rede->BarraInicial());
      }
   //importante: reordena lista de Barras de VTRedes
   VTListeqp *lisBAR = (VTListeqp*)(redes->LisBarra());
   lisBAR->Ordena();
   //destroi lista
   delete lisTMP;
   }

//---------------------------------------------------------------------------
void __fastcall TBatchCurva::RedefineId(TList *lisEQP)
   { //DVK 2016.04.12
   //variáveis locais
   VTEqpto *eqpto;
   VTCarga *carga;

   //loop p/ todos Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      eqpto->Status[sttNOVO] = true;
      eqpto->Id = FalsoId();
//      if (eqpto->StatusNovo()) eqpto->Id = FalsoId();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBatchCurva::RedefineIdCurvaPropria(TList *lisTMP)
   {//DVK 2016.04.13
   //variaveis locais
   VTCarga      *carga;
   VTEqpto      *eqpto;
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   for(int n = 0; n < lisTMP->Count; n++)
      {
      eqpto = (VTEqpto*)lisTMP->Items[n];
      //verifica se é carga com curva própria
      if(eqpto->Tipo() == eqptoCARGA)
         {
         carga = (VTCarga*)eqpto;
         if(! carga->curva->Tipica)
            {//redefine Id da curva própria
            carga->curva->Status[sttNOVO] = true;
            carga->curva->Id = FalsoId();
            continue;
            }
         }
      //verifica se é suprimento
      if(eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         suprimento = (VTSuprimento*)eqpto;
         //redefine Id da curva própria
         suprimento->curva->Status[sttNOVO] = true;
         suprimento->curva->Id = FalsoId();
         continue;
         }
      //verifica se é gerador
      if(eqpto->Tipo() == eqptoGERADOR)
         {
         gerador = (VTGerador*)eqpto;
         //redefine Id da curva própria
         gerador->curva->Status[sttNOVO] = true;
         gerador->curva->Id = FalsoId();
         continue;
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall TBatchCurva::SalvaBaseCurvaTipica(VTCurva *curva)
   {
   //variáveis locais
   VTBD     *bd     = (VTBD*)apl->GetObject(__classid(VTBD));
   VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   try{//verifica se existe BD
      if(! bd) apl->Add(bd = DLL_NewObjBD(apl));
      //conecta com a base
      if (! bd->Conectado) bd->Conecta();
      //verifica se é uma curva de carga
      if (ExisteCurva(curvas->ListaCurvaTipicaCarga(), curva))
         {
         bd->Geral->SalvaCurvaTipica(eqptoCARGA, curva);
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TBatchCurva::SalvaRedeNaBase(TDateTime data)
   {
   //variáveis locais
   AnsiString arq_rede;
   VTRedutor  *redutor;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica Abortar pedida pelo usuário
   //if (Abortar) return(false);
   //atualiza LBoxLog
   LBoxLog->Items->Add("> Gravando base da rede"); LBoxLog->Refresh();
   try{//cria objetos
		if ((redutor =  DLL_NewObjRedutor(apl)) == NULL) return(false);
		//proteção
		if(! salva) apl->Add(salva = DLL_NewObjSalva(NULL, apl));
      //configura mesnagens redutor
      redutor->ProgressoEnabled = false;
      redutor->AvisoEnabled     = false;
      //monta redes reduzidas
//      if (! redutor->ExecutaReducaoMNet()) return(false);
      //DVK 2016.04.06 salva mesmo se não conseguir montar equivalentes
      if (! redutor->ExecutaReducaoMNet())
         {int a = 0;}
      //destrói objeto VTRedutor
      delete redutor;
      salva->SalvaRede();
      //verifica interrupção pedida pelo usuário
      //if (Abortar) return(false);
      //verifica se deve montar uma base única
      /*
      if (BaseUnica)
         {//define nome da base da rede
         arq_rede.sprintf("RedeCompleta_%4d.%02d.mdb", YearOf(data), MonthOf(data));
         arq_rede = DirRede + "\\" + arq_rede;
         //verifica se já existe a base de dados
         if (FileExists(arq_rede))
            {//DVK 2016.04.12 redefine todos IDs
            RedefineId();
            path->ArqRede = arq_rede;
            //atualiza base de dados
            salva->SalvaRede();
            }
         else
            {//cria a nova base de dados Access
            if (! CriaBaseAccessDaRede(arq_rede)) return(false);
            //atualiza base de dados
            salva->SalvaRedeComo();
            }
         }
      else
         {//define nome da base da rede com o nome da Subestação
         arq_rede = CodigoRedeSupridora();
         arq_rede.sprintf("Rede%s_%4d.%02d.mdb", arq_rede.c_str(), YearOf(data), MonthOf(data));
         arq_rede = DirRede + "\\" + arq_rede;
         //cria uma nova base de dados Access
         if (! CriaBaseAccessDaRede(arq_rede)) return(false);
         //atualiza base de dados
         salva->SalvaRedeComo();
         }
      */
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
// eof
