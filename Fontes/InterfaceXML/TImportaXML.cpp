//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <DateUtils.hpp>
#include "TImportaXML.h"
#include "TAplImportaXML.h"
#include "VTFormato.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapSerie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImportaXML* NewObjImportaXML(VTApl *apl_owner)
   {
   return(new TImportaXML(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TImportaXML::TImportaXML(VTApl *apl_owner)
   {
   //cria objeto AplImportaXML
   apl = new TAplImportaXML(NULL, apl_owner);
   //cria objetos
   lisEQP  = new TList();
   //inicia valor default p/ posicionamento de Eqbar nas Barras
   dxbar =  4000;
   dybar = -4000;
   }

//---------------------------------------------------------------------------
__fastcall TImportaXML::~TImportaXML(void)
   {
   //destrói objetos
   if (apl) {delete apl; apl = NULL;}
   //destrói lista sem destruir objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   // destrói XML. Verificar se os outros objetos são detonados automaticamente

   //   unck@20080417
   //   Se não deletar o CodeGuard não reclama.
   //   if (xmlConcessAux) { delete xmlConcessAux; xmlConcessAux = NULL;}
   }

//-----------------------------------------------------------------------------
int __fastcall TImportaXML::CriaBarra(_di_IXMLTCoordId xmlCoordId)
   {
   //variáveis locais
   AnsiString      codigo;
   int             barra_id, barra_index;
   strBARRA        str;
   VTBarra         *barra;
   _di_IXMLTId     xmlId;
   _di_IXMLTCoord  xmlCoordUTM;    // Coordenada UTM
   _di_IXMLTCoord  xmlCoordESQ1;   // Coordenada Esquema 1
   _di_IXMLTCoord  xmlCoordESQ2;   // Coordenada Esquema 2

   //obtém objetos obrigatórios de MLTCoordId
   xmlCoordUTM  = xmlCoordId->Get_coordUTM();
   xmlCoordESQ1 = xmlCoordId->Get_coordESQ1();
   xmlCoordESQ2 = xmlCoordId->Get_coordESQ2();
   //obtém atributos de MLTCoordId
   barra_id    = 0;
   codigo      = xmlCoordId->Get_strCod();     //informação redundante a ser eliminada no futuro
   barra_index = xmlCoordId->Get_intCoordNdx();
   str.utm.x   = int(xmlCoordUTM->Get_dblX()  * 100);
   str.utm.y   = int(xmlCoordUTM->Get_dblY()  * 100);
   str.esq.x1  = int(xmlCoordESQ1->Get_dblX() * 100);
   str.esq.y1  = int(xmlCoordESQ1->Get_dblY() * 100);
   str.esq.x2  = int(xmlCoordESQ2->Get_dblX() * 100);
   str.esq.y2  = int(xmlCoordESQ2->Get_dblY() * 100);
   try{//verifica se MLTCoordId possui XMLTId
      if ((xmlId = xmlCoordId->Get_id()) != NULL)
         {
         barra_id = xmlId->Get_intId();
         codigo   = xmlId->Get_strCod();
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   if (barra_id != 0)
      {//procura Barra por ID
      barra = monta->ExisteBarra(barra_id);
      }
   else if ((str.utm.x > 0)&&(str.utm.y > 0))
      {//procura Barra por coord.utm
      barra = monta->ExisteBarraCoordUtm(str.utm.x, str.utm.y);
      }
   else if ((str.esq.x1 != 0)&&(str.esq.y1 != 0)&&(str.esq.x2 != 0)&&(str.esq.y2 != 0))
      {//procura barra por coord.esquemático
      barra = monta->ExisteBarraCoordEsq(str.esq.x1, str.esq.y1, str.esq.x2, str.esq.y2);
      }
   //verifica necessidade de criar uma nova barra
   if (barra == NULL)
       {//define os atributos de strBARRA ainda não preenchidos
       str.bar_id    = barra_id;
       str.codigo    = codigo;
       str.vnom      = 0;   // DEPOOPS! Tensão da barra fixa???
       str.extern_id = "";
       //str.index     = barra_index;
       //insere Barra em Monta
       barra         = (VTBarra*)monta->InsereBarraEqpto(str);
       }
   return(barra->Id);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TImportaXML::CriaCurvaTipica(_di_IXMLTCurvaTipica xmlCurva, bool bolCurvaCarga)
   {
   int                  intCurvaId;
   int                  num_pto        = 0;
   int                  dim_valor      = 0;
   _di_IXMLTId          xmlId          = NULL;
   _di_IXMLTPatamTipList xmlPatamTipList = NULL;
   _di_IXMLTPatamTip    xmlPatamTip    = NULL;
   _di_IXMLTHorMin      xmlHorMin      = NULL;
   _di_IXMLdouble_List   xmlValList      = NULL;
   VTCurva*             curva          = NULL;
   VTCurvas*            curvas         = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTClasses*           classes        = (VTClasses*)apl->GetObject(__classid(VTClasses));
   strPATAMAR           str_patamar;
   bool                 bolAbort       = false;

   // Evita explosões
   if (!curvas)                                                      return(NULL);
   if (!classes)                                                     return(NULL);
   if (!xmlCurva)                                                    return(NULL);

   if ((xmlId = xmlCurva->Get_id()) == NULL)                         return(NULL);
   if ((xmlPatamTipList = xmlCurva->Get_Patamar()) == NULL)           return(NULL);

   num_pto = xmlCurva->Get_intNumPat();
   if (num_pto != xmlPatamTipList->Count)                             return(NULL);

   // Como a curva é alocada antes de completar a montagem
   // O monta não tem métodos para inserção de curvas típicas
   curva                      = DLL_NewObjCurva();
   curva->Id                  = intCurvaId;
   curva->Codigo              = xmlId->Get_strCod();
   curva->Tipo                = xmlCurva->Get_intTpoCurva();
   curva->Tipica              = true;
   curva->Unidade             = xmlCurva->Get_intUnidade();
   curva->Escala              = xmlCurva->Get_intEscala();
   curva->Classe              = NULL;
   curva->Status[sttNOVO]     = true;
   curva->Status[sttALTERADO] = false;
   // Parâmetros especiais da curva de carga
   if (bolCurvaCarga)
      {// Classe de consumidor
      curva->Classe         = classes->ExisteClasse(xmlCurva->Get_intTpoCons());
      // Armazena os valores de consumo mínimo e máximo
      curva->LimiteInferior = xmlCurva->Get_dblEneMin();
      curva->LimiteSuperior = xmlCurva->Get_dblEneMax();
      }
   memset(&(str_patamar), 0, sizeof(str_patamar));
   for (int index = 0; index < num_pto; index++)
      {
      if ((xmlPatamTip = xmlPatamTipList->Get_Item(index)) == NULL)   { bolAbort = true; break; }
      if ((xmlHorMin = xmlPatamTip->Get_horIni()) == NULL)           { bolAbort = true; break; }
      str_patamar.hm_ini.hora   = xmlHorMin->Get_intHor();
      str_patamar.hm_ini.minuto = xmlHorMin->Get_intMin();
      if ((xmlHorMin = xmlPatamTip->Get_horFim()) == NULL)           { bolAbort = true; break; }
      str_patamar.hm_fim.hora   = xmlHorMin->Get_intHor();
      str_patamar.hm_fim.minuto = xmlHorMin->Get_intMin();
      // Obtém os valores de cada patamar
      if ((xmlValList = xmlPatamTip->Get_dblVal()) == NULL)           { bolAbort = true; break; }
      dim_valor = xmlPatamTip->Get_intNumVal();
      // Verificação contra explosões
      if (dim_valor >= SIZEOF_VALOR)                                 { bolAbort = true; break; }
      for (int n = 0; n < dim_valor; n++)
         {
         str_patamar.valor[n] = xmlValList->Get_Item(n);
         }
      curva->NumeroValores = dim_valor;
      if (!curva->SetPonto(str_patamar.hm_ini, str_patamar.hm_fim, str_patamar.valor, dim_valor))
         {
         bolAbort = true;
         break;
         }
      }

   // Garante que o ponteiro não vai ficar voando
   if (bolAbort)
      {
      if (curva) { delete(curva); curva = NULL; }
      }
   // Retorna a curva criada
   return(curva);
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TImportaXML::DefineCodigoUnicoArranjo(AnsiString cod_orig)
   {
   //variáveis locais
   int count= 1;
   AnsiString cod_alt;

   //verifica se não existe arranjo com o código indicado
   if (arranjos->ExisteArranjo(cod_orig) == NULL) return(cod_orig);
   //código já existe: redefine o código
   do {
      cod_alt = cod_orig + " *" + IntToStr(count++);
      }while (arranjos->ExisteArranjo(cod_alt) != NULL);
   return(cod_alt);
   }

//-----------------------------------------------------------------------------
bool __fastcall TImportaXML::ExecutaAvaliacao(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso = false;
   VTPath *path;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   formato  = (VTFormato*)apl->GetObject(__classid(VTFormato));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   //abre log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//valida estrutura do arquivo XML
         sucesso =  formato->Executa(xmlConcessAux);
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("A estrutura do arquivo XML está correta.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("A estrutura do arquivo XML não está correta.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::ExecutaImportacao(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso;
   VTPath  *path;
   VTRedes *redes;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   monta    = (VTMonta*)apl->GetObject(__classid(VTMonta));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //abre aqruivo de log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//insere Rede e seus eqptos a partir dos objetos XML
         sucesso              = InsereConjuntoArranjo();
         if (sucesso) sucesso = InsereConjuntoCurvasTipicas();
         if (sucesso) sucesso = InsereRedes();
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //executa montagem da rede
   if ((redes->LisRede())->Count > 0)
      {
      monta->RedefineId();
      monta->TrataFimDeDados(true);
      }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("O arquivo XML foi lido corretamente.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("Erro na leitura do arquivo XML.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::ImportaArranjos(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso;
   VTPath  *path;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   monta    = (VTMonta*)apl->GetObject(__classid(VTMonta));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   //abre aqruivo de log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//insere Cabos e Arranjos
         sucesso = InsereConjuntoArranjo();
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("O arquivo XML foi lido corretamente.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("Erro na leitura do arquivo XML.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::ImportaConfiguracao(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso;
   VTPath  *path;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   monta    = (VTMonta*)apl->GetObject(__classid(VTMonta));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   //abre aqruivo de log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//insere Cabos e Arranjos
         sucesso              = InsereConjuntoArranjo();
         if (sucesso) sucesso = InsereConjuntoCurvasTipicas();
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("O arquivo XML foi lido corretamente.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("Erro na leitura do arquivo XML.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::ImportaCurvasTipicas(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso;
   VTPath  *path;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   monta    = (VTMonta*)apl->GetObject(__classid(VTMonta));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   //abre aqruivo de log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//insere Cabos e Arranjos
         sucesso = InsereConjuntoCurvasTipicas();
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("O arquivo XML foi lido corretamente.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("Erro na leitura do arquivo XML.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::ImportaRedesApenas(AnsiString arq_xml)
   {
   //variáveis locais
   bool    sucesso;
   VTPath  *path;
   VTRedes *redes;

   //obtém objetos de Apl
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   monta    = (VTMonta*)apl->GetObject(__classid(VTMonta));
   path     = (VTPath*)apl->GetObject(__classid(VTPath));
   plog     = (VTLog*)apl->GetObject(__classid(VTLog));
   fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //abre aqruivo de log
   plog->Open(path->DirTmp() + "\\ImportaXML.log");
   try{//carrega arquivo XML
      if ((xmlConcessAux = LoadConcessionaria(arq_xml)) != NULL)
         {//insere somente as Redes
         //sucesso              = InsereConjuntoArranjo();
         //if (sucesso) sucesso = InsereConjuntoCurvasTipicas();
         sucesso = InsereRedes();
         }
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //executa montagem da rede
   if ((redes->LisRede())->Count > 0)
      {
      monta->RedefineId();
      monta->TrataFimDeDados(true);
      }
   //inclui mensagem final no log
   if (sucesso)
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("O arquivo XML foi lido corretamente.");
      }
   else
      {
      plog->Add(AnsiString::StringOfChar('-', 80));
      plog->Add("Erro na leitura do arquivo XML.");
      }
   //fecha o log
   plog->Close();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoArranjo(void)
   {
   //variáveis locais
   bool                        sucesso;
   _di_IXMLTCaboRGMRList        lstCaboRGMR;
   _di_IXMLTCaboZ0Z1List        lstCaboZ0Z1;
   _di_IXMLTSuporteList         lstSuporte;
   _di_IXMLTArranjoZ0Z1List     lstArranjoZ0Z1;
   _di_IXMLTArranjoCaboZ0Z1List lstArranjoCaboZ0Z1;
   _di_IXMLTArranjoCaboRGMRList lstArranjoCaboRGMR;

   //insere conjunto de Cabo monopolar
   lstCaboRGMR = xmlConcessAux->Get_CaboRGMR();
   sucesso     = InsereConjuntoCaboMonopolar(lstCaboRGMR);
   lstCaboRGMR->Clear();
   //insere conjunto de Cabo multiplexado
   if (sucesso)
      {
      lstCaboZ0Z1 = xmlConcessAux->Get_CaboZ0Z1();
      sucesso     = InsereConjuntoCaboMultiplexado(lstCaboZ0Z1);
      lstCaboZ0Z1->Clear();
      }
   //insere conjunto de Suporte
   if (sucesso)
      {
      lstSuporte = xmlConcessAux->Get_Suporte();
      sucesso    = InsereConjuntoSuporte(lstSuporte);
      lstSuporte->Clear();
      }

   //insere conjunto de Arranjos
   if (sucesso)
      {
      lstArranjoZ0Z1 = xmlConcessAux->Get_ArranjoZ0Z1();
      sucesso = InsereConjuntoArranjoZ0Z1(lstArranjoZ0Z1);
      lstArranjoZ0Z1->Clear();

      if (sucesso)
         {
         lstArranjoCaboZ0Z1 = xmlConcessAux->Get_ArranjoCaboZ0Z1();
         sucesso = InsereConjuntoArranjoCaboZ0Z1(lstArranjoCaboZ0Z1);
         lstArranjoCaboZ0Z1->Clear();

         if (sucesso)
            {
            lstArranjoCaboRGMR = xmlConcessAux->Get_ArranjoCaboRGMR();
            sucesso = InsereConjuntoArranjoCaboRGMR(lstArranjoCaboRGMR);
            lstArranjoCaboRGMR->Clear();
            }
         }
      }
   return(sucesso);
   }


//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoArranjoCaboRGMR(_di_IXMLTArranjoCaboRGMRList lstArranjo)
   {
   _di_IXMLTArranjoCaboRGMRList   xmlArranjoList;
   _di_IXMLTArranjoCaboRGMR      xmlArranjo;
   _di_IXMLTSuportePtoFixCabo    xmlPtoFix;
   _di_IXMLTSuportePtoFixCaboList xmlPtoFixList;
   _di_IXMLTId                   xmlId;
   strARRANJO2                   str_arranjo;
   int                           intFase;
   int                           ndxFase;

   // Se não houver a lista, não é erro
   if (!lstArranjo) return(true);
   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstArranjo->Count; i++)
      {
      if ((xmlArranjo   = lstArranjo->Get_Item(i))  == NULL) return(false);
      if ((xmlId        = xmlArranjo->Get_id())     == NULL) return(false);
      if ((xmlPtoFixList = xmlArranjo->Get_PtoFix()) == NULL) return(false);

      // Configura os valores dos campos
      str_arranjo.arranjo_id                  = xmlId->Get_intId();
      str_arranjo.codigo                      = xmlId->Get_strCod();
      str_arranjo.tipo_cfg                    = arranjoCABOxFASE;   //ver VTArranjo::enumCFG_ARRANJO
      str_arranjo.tipico                      = true;
      str_arranjo.res_terra                   = 0.;
      str_arranjo.vmax_kv                     = 0.;
      str_arranjo.vmin_kv                     = 0.;
      str_arranjo.fases                       = fases->ExisteFase(xmlArranjo->Get_strFases());
      str_arranjo.status                      = sttNOVO;
      str_arranjo.cfg_suporte_cabo.suporte_id = xmlArranjo->Get_intSuporteId();

      if (xmlPtoFixList->Count > MAX_FASE) return(false);
      // Zera as estruturas
      for (int i = 0; i < MAX_FASE; i++)
         {
         str_arranjo.cfg_suporte_cabo.fase[i].cabo_id        = -1;
         str_arranjo.cfg_suporte_cabo.fase[i].pto_fixacao_id = -1;
         str_arranjo.cfg_suporte_cabo.fase[i].multiplicidade =  1;
         }
      // Agora preenche com os valoes válidos
      for (int i = 0; i < xmlPtoFixList->Count; i++)
         {
         if ((xmlPtoFix = xmlPtoFixList->Get_Item(i)) == NULL) return(false);
         // Determina a fase em questão
         intFase = fases->ExisteFase(xmlPtoFix->Get_strFase());
         if      ((intFase & faseA) == faseA) ndxFase = 0;
         else if ((intFase & faseB) == faseB) ndxFase = 1;
         else if ((intFase & faseC) == faseC) ndxFase = 2;
         else if ((intFase & faseN) == faseN) ndxFase = 3;
         else return(false);

         // str_arranjo.cfg_suporte_cabo.fase.
         str_arranjo.cfg_suporte_cabo.fase[ndxFase].cabo_id        = xmlPtoFix->Get_intCaboId();
         str_arranjo.cfg_suporte_cabo.fase[ndxFase].pto_fixacao_id = xmlPtoFix->Get_intId();
         str_arranjo.cfg_suporte_cabo.fase[ndxFase].multiplicidade =  1;
         }
      //garante arranjo com código único
      str_arranjo.codigo = DefineCodigoUnicoArranjo(str_arranjo.codigo);
      //insere o arranjo
      monta->InsereArranjo(str_arranjo);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoArranjoCaboZ0Z1(_di_IXMLTArranjoCaboZ0Z1List lstArranjo)
   {
   _di_IXMLTArranjoCaboZ0Z1List xmlArranjoList;
   _di_IXMLTArranjoCaboZ0Z1    xmlArranjo;
   _di_IXMLTId                 xmlId;
   strARRANJO2                 str_arranjo;

   // Se não houver a lista, não é erro
   if (!lstArranjo) return(true);
   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstArranjo->Count; i++)
      {
      if ((xmlArranjo = lstArranjo->Get_Item(i))   == NULL) return(false);
      if ((xmlId      = xmlArranjo->Get_id())      == NULL) return(false);
      // Configura os valores dos campos
      str_arranjo.arranjo_id           = xmlId->Get_intId();
      str_arranjo.codigo               = xmlId->Get_strCod();
      str_arranjo.tipo_cfg             = arranjoCABO_Z0Z1;   //ver VTArranjo::enumCFG_ARRANJO
      str_arranjo.tipico               = true;
      str_arranjo.fases                = fases->ExisteFase(xmlArranjo->Get_strFases());
      str_arranjo.status               = sttNOVO;
      str_arranjo.cfg_caboZOZ1.cabo_id = xmlArranjo->Get_intCaboId();
      str_arranjo.res_terra            = 0.;
      str_arranjo.vmax_kv              = 0.;
      str_arranjo.vmin_kv              = 0.;
      //garante arranjo com código único
      str_arranjo.codigo = DefineCodigoUnicoArranjo(str_arranjo.codigo);
      //insere o arranjo
      monta->InsereArranjo(str_arranjo);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TImportaXML::InsereConjuntoArranjoZ0Z1(_di_IXMLTArranjoZ0Z1List lstArranjo)
   {
   _di_IXMLTArranjoZ0Z1 xmlArranjo;
   _di_IXMLTId          xmlId;
   _di_IXMLTCSeq        xmlCSeq;
   _di_IXMLTZseq        xmlZSeq;
   _di_IXMLTZ           xmlZ0;
   _di_IXMLTZ           xmlZ1;
   strARRANJO2          str_arranjo;

   // Se não houver a lista, não é erro
   if (!lstArranjo) return(true);
   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstArranjo->Count; i++)
      {
      if ((xmlArranjo = lstArranjo->Get_Item(i))   == NULL) return(false);
      if ((xmlId      = xmlArranjo->Get_id())      == NULL) return(false);
      if ((xmlCSeq    = xmlArranjo->Get_dblCSeq()) == NULL) return(false);
      if ((xmlZSeq    = xmlArranjo->Get_zSeq())    == NULL) return(false);
      if ((xmlZ0      = xmlZSeq->Get_z0())         == NULL) return(false);
      if ((xmlZ1      = xmlZSeq->Get_z1())         == NULL) return(false);

      // Configura os valores dos campos
      str_arranjo.arranjo_id               = xmlId->Get_intId();
      str_arranjo.codigo                   = xmlId->Get_strCod();
      str_arranjo.tipo_cfg                 = arranjoZ0Z1;   //ver VTArranjo::enumCFG_ARRANJO
      str_arranjo.tipico                   = true;
      str_arranjo.fases                    = fases->ExisteFase(xmlArranjo->Get_strFases());
      str_arranjo.status                   = sttNOVO;
      str_arranjo.cfg_arranjoZ0Z1.iadm_amp = xmlArranjo->Get_dblCorMax();
      str_arranjo.cfg_arranjoZ0Z1.z0.r     = xmlZ0->Get_dblR();
      str_arranjo.cfg_arranjoZ0Z1.z0.x     = xmlZ0->Get_dblX();
      str_arranjo.cfg_arranjoZ0Z1.z1.r     = xmlZ1->Get_dblR();
      str_arranjo.cfg_arranjoZ0Z1.z1.x     = xmlZ1->Get_dblX();
      str_arranjo.cfg_arranjoZ0Z1.c0       = xmlCSeq->Get_dblC0();
      str_arranjo.cfg_arranjoZ0Z1.c1       = xmlCSeq->Get_dblC1();
      str_arranjo.res_terra                = 0.;
      str_arranjo.vmax_kv                  = 0.;
      str_arranjo.vmin_kv                  = 0.;
      //garante arranjo com código único
      str_arranjo.codigo = DefineCodigoUnicoArranjo(str_arranjo.codigo);
      //insere o arranjo
      monta->InsereArranjo(str_arranjo);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCaboMonopolar(_di_IXMLTCaboRGMRList lstCabo)
   {
   //variáveis locais
   _di_IXMLTCaboRGMR xmlCabo;
   _di_IXMLTId       xmlId;          // Id do equipamento
   strCABO           str_cabo;       // Estrutura de armazenamento a ser inserida em TMonta

   //proteção: verifica o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (! lstCabo) return(true);
   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstCabo->Count; i++)
      {
      if ((xmlCabo = lstCabo->Get_Item(i)) == NULL) return(false);
      if ((xmlId   = xmlCabo->Get_id())    == NULL) return(false);
      //lê dados do cabo
      str_cabo.id        = xmlId->Get_intId();
      str_cabo.codigo    = xmlId->Get_strCod();
      str_cabo.tipo_cfg  = caboR_GMR;
      str_cabo.status    = sttNOVO;
      str_cabo.iadm      = xmlCabo->Get_dblCorMax();
      str_cabo.vmin      = 0;
      str_cabo.vmax      = 0;
      str_cabo.r         = xmlCabo->Get_dblR();
      str_cabo.raio      = 0;
      str_cabo.gmr       = xmlCabo->Get_dblGMR(); //mm
      str_cabo.z0.r      = 0;
      str_cabo.z0.x      = 0;
      str_cabo.z1.r      = 0;
      str_cabo.z1.x      = 0;
      str_cabo.c0        = 0;
      str_cabo.c1        = 0;
      //insere cabo
      monta->InsereCabo(str_cabo);
      //destrói objetos XML
      //delete xmlCabo;
      //delete xmlId;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCaboMultiplexado(_di_IXMLTCaboZ0Z1List lstCabo)
   {
   //variáveis locais
   _di_IXMLTCaboZ0Z1 xmlCabo;
   _di_IXMLTId       xmlId;          // Id do equipamento
   _di_IXMLTCSeq     xmlCSeq;        // Capacitância de seqüência
   _di_IXMLTZseq     xmlZSeq;        // Impedância de seqüência
   _di_IXMLTZ        xmlZ0, xmlZ1;   // Impedância
   strCABO           str_cabo;       // Estrutura de armazenamento a ser inserida em TMonta

   //proteção: verifica o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (! lstCabo) return(true);
   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstCabo->Count; i++)
      {
      if ((xmlCabo = lstCabo->Get_Item(i))   == NULL) return(false);
      if ((xmlId   = xmlCabo->Get_id())      == NULL) return(false);
      if ((xmlCSeq = xmlCabo->Get_dblCSeq()) == NULL) return(false);
      if ((xmlZSeq = xmlCabo->Get_zSeq())    == NULL) return(false);
      //lê dados do cabo
      str_cabo.id        = xmlId->Get_intId();
      str_cabo.codigo    = xmlId->Get_strCod();
      str_cabo.tipo_cfg  = caboZ0Z1;
      str_cabo.status    = sttNOVO;
      str_cabo.iadm      = xmlCabo->Get_dblCorMax();
      str_cabo.vmin      = 0;
      str_cabo.vmax      = 0;
      str_cabo.r         = 0;
      str_cabo.raio      = 0;
      str_cabo.gmr       = 0;
      //impedâncias de seq.0
      xmlZ0              = xmlZSeq->Get_z0();
      str_cabo.z0.r      = xmlZ0->Get_dblR();
      str_cabo.z0.x      = xmlZ0->Get_dblX();
      //impedâncias de seq.1
      xmlZ1              = xmlZSeq->Get_z1();
      str_cabo.z1.r      = xmlZ1->Get_dblR();
      str_cabo.z1.x      = xmlZ1->Get_dblX();
      //capacitâncias
      str_cabo.c0        = xmlCSeq->Get_dblC0();
      str_cabo.c1        = xmlCSeq->Get_dblC1();
      //insere cabo
      monta->InsereCabo(str_cabo);
      //destrói objetos XML
      //delete xmlCabo;
      //delete xmlId;
      //delete xmlCSeq;
      //delete xmlZSeq;
      //delete xmlZ0;
      //delete xmlZ1;
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereEstudo(void)
   {
   //variáveis locais
   strPLAN_ESTUDO str_estudo;

   //inicia dados do Estudo
   str_estudo.estudo_id = -1;
   str_estudo.tipo      = estudoREDE_ORIGINAL;
   str_estudo.codigo    = "Rede Original";
   str_estudo.autor     = "";
   str_estudo.data      = DateOf(Now());
   return(monta->InsereEstudo(str_estudo));
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereRedes(void)
   {
   //variáveis locais
   bool                 sucesso;
   _di_IXMLTRedeList     lstRede;

   lstRede = xmlConcessAux->Get_Rede();
   sucesso = InsereConjuntoRede(lstRede);
   lstRede->Clear();

   return(sucesso);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TImportaXML::InsereCluster(AnsiString codigo)
   {
   //variáveis locais
   strCLUSTER str_cluster;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //insere Cluster
   str_cluster.obra       = false;
   str_cluster.cluster_id = FalsoId();
   str_cluster.codigo     = codigo;
   return(monta->InsereCluster(str_cluster));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TImportaXML::InsereDominio(AnsiString codigo)
   {
   //variáveis locais
   strDOMINIO str_dominio;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //insere Cluster
   str_dominio.obra       = false;
   str_dominio.dominio_id = -1;
   str_dominio.codigo     = codigo;
   return(monta->InsereDominio(str_dominio));
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCapacitor(_di_IXMLTEqBarCapacitorList lstCapacitor, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strCAPACITOR                str_capacitor;  // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarCapacitor     xmlCapacitor;   // Capacitor
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstCapacitor) return(true);

   for (int i = 0; i < lstCapacitor->Count; i++)
      {
      if ((xmlCapacitor = lstCapacitor->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto     = xmlCapacitor->Get_eqbar()) == NULL) return(false);
      if ((xmlId        = xmlEqpto->Get_id())        == NULL) return(false);

      str_capacitor.cap_id     = xmlId->Get_intId();
      str_capacitor.codigo     = xmlId->Get_strCod();
      str_capacitor.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 1) return(false);

      xmlCoordId               = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM              = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1             = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2             = xmlCoordId->Get_coordESQ2();
      str_capacitor.bar_id     = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                           xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                           xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                           xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_capacitor.bar_id     = CriaBarra(xmlCoordId);
      str_capacitor.esq.posbar = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordIco              = xmlCoordId->Get_coordIco();
      str_capacitor.esq.dxbar  = xmlCoordIco->Get_dblDxBar();   // unck@20080807
      str_capacitor.esq.dybar  = xmlCoordIco->Get_dblDyBar();   // unck@20080807

      str_capacitor.vnom       = xmlCapacitor->Get_dblTnsNom();
      str_capacitor.q_mvar     = xmlCapacitor->Get_dblPotRtv();
      str_capacitor.ligacao    = lgTRIANG;
      //preenche dados da curva
      str_capacitor.curva.id        = -1;
      str_capacitor.curva.tipica    = true;
      str_capacitor.curva.status    = 0;
      str_capacitor.curva.num_pat   = 0;
      str_capacitor.curva.num_valor = nvCURVA_PQ;
      // Insere Capacitor em Monta
      //if ((eqpto = monta->InsereCapacitor(str_capacitor)) == NULL) return(false);
      if ((eqpto = monta->InsereCapacitor(str_capacitor)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCapserie(_di_IXMLTEqLigCapacitorSerieList lstCapacitorSerie, VTRede* rede)
   {
   VTEqpto*                      eqpto;
   strCAPSERIE                   str_capacitorserie;   // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigCapacitorSerie  xmlCapacitorSerie;    // XML - Capacitor série
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoord1_Id;         // Coordenada identificada (Barras)
   _di_IXMLTCoord                xmlCoord1_UTM;        // Coordenada UTM
   _di_IXMLTCoord                xmlCoord1_ESQ1;       // Coordenada Esquema 1
   _di_IXMLTCoord                xmlCoord1_ESQ2;       // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord1_Ico;        // Deslocamentos para posicionamento dos ícones
   _di_IXMLTCoordId              xmlCoord2_Id;         // Coordenada identificada (Barras)
   _di_IXMLTCoord                xmlCoord2_UTM;        // Coordenada UTM
   _di_IXMLTCoord                xmlCoord2_ESQ1;       // Coordenada Esquema 1
   _di_IXMLTCoord                xmlCoord2_ESQ2;       // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord2_Ico;        // Deslocamentos para posicionamento dos ícones
   _di_IXMLTZ                    xmlZ;                 // XML - Impedância

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstCapacitorSerie) return(true);
     

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede)  return(false);
     
   for (int i = 0; i < lstCapacitorSerie->Count; i++)
      {
      if ((xmlCapacitorSerie = lstCapacitorSerie->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto          = xmlCapacitorSerie->Get_eqlig()) == NULL) return(false);
      if ((xmlId             = xmlEqpto->Get_id())             == NULL) return(false);

      str_capacitorserie.id      = xmlId->Get_intId();
      str_capacitorserie.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 2) return(false);

      xmlCoord1_Id                 = lstCoordId->Get_Item(0);
      xmlCoord1_UTM                = xmlCoord1_Id->Get_coordUTM();
      xmlCoord1_ESQ1               = xmlCoord1_Id->Get_coordESQ1();
      xmlCoord1_ESQ2               = xmlCoord1_Id->Get_coordESQ2();
      /*
      str_capacitorserie.bar1_id = CriaBarra(xmlCoord1_Id->Get_intCoordNdx(), xmlCoord1_Id->Get_strCod(),
                                             xmlCoord1_UTM->Get_dblX(),       xmlCoord1_UTM->Get_dblY(),
                                             xmlCoord1_ESQ1->Get_dblX(),      xmlCoord1_ESQ1->Get_dblY(),
                                             xmlCoord1_ESQ2->Get_dblX(),      xmlCoord1_ESQ2->Get_dblY());
      */
      str_capacitorserie.bar1_id = CriaBarra(xmlCoord1_Id);
      str_capacitorserie.esq.posbar1 = xmlCoord1_Id->Get_dblPosBar();  // unck@20080807

      xmlCoord2_Id                 = lstCoordId->Get_Item(1);
      xmlCoord2_UTM                = xmlCoord2_Id->Get_coordUTM();
      xmlCoord2_ESQ1               = xmlCoord2_Id->Get_coordESQ1();
      xmlCoord2_ESQ2               = xmlCoord2_Id->Get_coordESQ2();
      /*
      str_capacitorserie.bar2_id = CriaBarra(xmlCoord2_Id->Get_intCoordNdx(), xmlCoord2_Id->Get_strCod(),
                                             xmlCoord2_UTM->Get_dblX(),       xmlCoord2_UTM->Get_dblY(),
                                             xmlCoord2_ESQ1->Get_dblX(),      xmlCoord2_ESQ1->Get_dblY(),
                                             xmlCoord2_ESQ2->Get_dblX(),      xmlCoord2_ESQ2->Get_dblY());
      */
      str_capacitorserie.bar2_id = CriaBarra(xmlCoord2_Id);

      str_capacitorserie.esq.posbar2     = xmlCoord2_Id->Get_dblPosBar();  // unck@20080807
      str_capacitorserie.codigo          = xmlId->Get_strCod();
      str_capacitorserie.extern_id       = xmlId->Get_strExtId();               // Código externo. Inicialmente o mesmo código do capacitorserie
      str_capacitorserie.vnom            = xmlCapacitorSerie->Get_dblTnsNom();
      str_capacitorserie.snom            = xmlCapacitorSerie->Get_dblPotNom();

      xmlZ                               = xmlCapacitorSerie->Get_z();
      str_capacitorserie.z.r             = xmlZ->Get_dblR();
      str_capacitorserie.z.x             = xmlZ->Get_dblX();

      str_capacitorserie.esq.posbar3     = 0;
      str_capacitorserie.esq.comprimento = 0;

      // Insere o capacitor série em monta
      if ((eqpto = monta->InsereCapserie(str_capacitorserie)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      //destrói objetos XML
      //delete xmlCapacitorSerie;
      //delete xmlEqpto;
      //delete xmlId;
      //delete lstCoordId;
      //delete xmlCoord1_Id  ;
      //delete xmlCoord1_UTM ;
      //delete xmlCoord1_ESQ1;
      //delete xmlCoord1_ESQ2;
      //delete xmlCoord2_Id  ;
      //delete xmlCoord2_UTM ;
      //delete xmlCoord2_ESQ1;
      //delete xmlCoord2_ESQ2;
      //delete xmlZ;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCarga(_di_IXMLTEqBarCargaList lstCarga, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strCARGA                    str_carga;      // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarCarga         xmlCarga;       // Carga (Típica ou Especial)
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones
   _di_IXMLTPatamPropList       lstPatamar;     // Lista de patamares
   _di_IXMLTPatamProp          xmlPatamar;     // Lista de patamares
   _di_IXMLTHorMin             xmlhm_ini;      // Hora e Minuto inicial do patamar
   _di_IXMLTHorMin             xmlhm_fim;      // Hora e Minuto final   do patamar

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstCarga) return(true);
   //loop p/ todas Cargas
   for (int i = 0; i < lstCarga->Count; i++)
      {
      try{
         if ((xmlCarga = lstCarga->Get_Item(i)) == NULL) return(false);
         if ((xmlEqpto = xmlCarga->Get_eqbar()) == NULL) return(false);
         if ((xmlId    = xmlEqpto->Get_id())    == NULL) return(false);
         str_carga.car_id     = xmlId->Get_intId();
         str_carga.codigo     = xmlId->Get_strCod();
         //str_carga.extern_id  = xmlId->Get_strExtId();
         //str_carga.extern_id = "";
         lstCoordId = xmlEqpto->Get_coordId();
         // Verifica o número de coordenadas esperado.
         if (lstCoordId->Count < 1) return(false);

         xmlCoordId             = lstCoordId->Get_Item(0);
         /*
         xmlCoordUTM            = xmlCoordId->Get_coordUTM();
         xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
         xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
         str_carga.bar_id       = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
         */
         str_carga.bar_id       = CriaBarra(xmlCoordId);
         str_carga.esq.posbar   = xmlCoordId->Get_dblPosBar();  // unck@20080807

         xmlCoordIco            = xmlCoordId->Get_coordIco();
         str_carga.esq.dxbar    = xmlCoordIco->Get_dblDxBar();   // unck@20080807
         str_carga.esq.dybar    = xmlCoordIco->Get_dblDyBar();   // unck@20080807
         //modelo de carga
         str_carga.modelo.icte_pu = 0;
         str_carga.modelo.scte_pu = 0;
         str_carga.modelo.zcte_pu = 0;
         switch(xmlCarga->Get_intTpoModCrg())
            {
            case cargaICTE: str_carga.modelo.icte_pu = 1.0; break;
            case cargaSCTE: str_carga.modelo.scte_pu = 1.0; break;
            case cargaZCTE: str_carga.modelo.zcte_pu = 1.0; break;
            default:        str_carga.modelo.icte_pu = 1.0; break;
            }

         str_carga.classe_cons    = xmlCarga->Get_intTpoCons();   // Utilizado em cargas típicas
         str_carga.energia_mwhmes = xmlCarga->Get_dblEne();       // Utilizado em cargas típicas
         //str_carga.fases        = xmlCarga->Get_strFases();     // unck@20090721
         str_carga.fases          = monta->ExisteFase(xmlCarga->Get_strFases());
         str_carga.ip.num_lamp    = 0;
         str_carga.ip.pot_kw      = 0;
         str_carga.ip.fat_pot     = 0;
         //número de consumidores
         str_carga.num_cons.residencial = 0;
         str_carga.num_cons.comercial   = 0;
         str_carga.num_cons.industrial  = 0;
         str_carga.num_cons.rural       = 0;
         str_carga.num_cons.outros      = 0;
         str_carga.num_cons.a4          = 0;
         switch(str_carga.classe_cons)
            {
            case consRESIDENCIAL: str_carga.num_cons.residencial = 1; break;
            case consCOMERCIAL:   str_carga.num_cons.comercial   = 1; break;
            case consINDUSTRIAL:  str_carga.num_cons.industrial  = 1; break;
            case consRURAL:       str_carga.num_cons.rural       = 1; break;
            case consOUTROS:      str_carga.num_cons.outros      = 1; break;
            case consA4:          str_carga.num_cons.a4          = 1; break;
            case consIP:          /*nada a fazer*/                  ; break;
            case consEQUIVALENTE://lê dados do número de consumidores
               str_carga.num_cons.residencial = 0;
               str_carga.num_cons.comercial   = 0;
               str_carga.num_cons.industrial  = 0;
               str_carga.num_cons.rural       = 0;
               str_carga.num_cons.outros      = 0;
               str_carga.num_cons.a4          = 0;
               break;
            }
         // As curvas são obrigatórias na carga específica, porém não na típica
         lstPatamar = xmlCarga->Get_Patamar();
         if ((lstPatamar == NULL)||(lstPatamar->Count == 0))
            {//curva típica
            str_carga.curva.id        = -1;
            str_carga.curva.tipica    = true;
            str_carga.curva.num_valor = nvCURVA_PQ;
            str_carga.curva.num_pat   = 0;
            }
         else
            {//curva própria: Carrega os patamares
            str_carga.curva.id        = -1;
            str_carga.curva.tipica    = false;
            str_carga.curva.num_valor = nvCURVA_PQ;
            str_carga.curva.num_pat   = lstPatamar->Count;
            for (int j = 0; j < lstPatamar->Count; j++)
               {
               xmlPatamar = lstPatamar->Get_Item(j);
               xmlhm_ini  = xmlPatamar->Get_horIni();
               str_carga.curva.patamar[j].hm_ini.hora   = xmlhm_ini->Get_intHor();
               str_carga.curva.patamar[j].hm_ini.minuto = xmlhm_ini->Get_intMin();
               xmlhm_fim = xmlPatamar->Get_horFim();
               str_carga.curva.patamar[j].hm_fim.hora   = xmlhm_fim->Get_intHor();
               str_carga.curva.patamar[j].hm_fim.minuto = xmlhm_fim->Get_intMin();
               str_carga.curva.patamar[j].valor[indP] = xmlPatamar->Get_dblPotAtv() / 1000.; //kW   => MW
               str_carga.curva.patamar[j].valor[indQ] = xmlPatamar->Get_dblPotRtv() / 1000.; //kVAr => MVAr;
               str_carga.curva.patamar[j].valor[indV] = 0.;
               str_carga.curva.patamar[j].valor[indT] = 0.;
               }
            }
         // Insere Carga Específica em Monta
         if ((eqpto = monta->InsereCarga(str_carga)) == NULL) continue;
         // Insere o conjunto de medições
         if (! InsereMedicao(xmlEqpto, eqpto)) continue;
         }catch(Exception &e)
            {
            MsgLog("Erro na leitura dos dados da carga " + IntToStr(i+1), rede);
            return(false);
            }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCargaIP(_di_IXMLTEqBarCargaIPList lstCargaIP, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strCARGA                    str_carga;      // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarCargaIP       xmlCarga;       // Carga (Típica ou Especial)
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstCargaIP) return(true);

   for (int i = 0; i < lstCargaIP->Count; i++)
      {
      if ((xmlCarga = lstCargaIP->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto = xmlCarga->Get_eqbar())   == NULL) return(false);
      if ((xmlId    = xmlEqpto->Get_id())      == NULL) return(false);

      str_carga.car_id     = xmlId->Get_intId();
      str_carga.codigo     = xmlId->Get_strCod();
      //str_carga.extern_id  = xmlId->Get_strExtId();
      //str_carga.extern_id  = "";

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 1) return(false);

      xmlCoordId             = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM            = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
      str_carga.bar_id       = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                         xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                         xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                         xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_carga.bar_id       = CriaBarra(xmlCoordId);
      str_carga.esq.posbar   = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordIco            = xmlCoordId->Get_coordIco();
      str_carga.esq.dxbar    = xmlCoordIco->Get_dblDxBar();   // unck@20080807
      str_carga.esq.dybar    = xmlCoordIco->Get_dblDyBar();   // unck@20080807
      //identifica como carga de IP
      str_carga.classe_cons = consIP;
      str_carga.ip.num_lamp = xmlCarga->Get_intNumLamp();     // unck@20090721
      str_carga.ip.pot_kw   = xmlCarga->Get_dblPotAtv();      // unck@20090721
      str_carga.ip.fat_pot  = xmlCarga->Get_dblFtrPot();      // unck@20090721
      //str_carga.fases        = xmlCarga->Get_strFases();       // unck@20090721
      str_carga.fases       = monta->ExisteFase(xmlCarga->Get_strFases());       // unck@20090721

      // Insere Carga Específica em Monta
      if ((eqpto = monta->InsereCarga(str_carga)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoChave(_di_IXMLTEqLigChaveList lstChave, VTRede* rede)
   {
   VTEqpto*                      eqpto;
   strCHAVE                      str_chave;            // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigChave           xmlChave;             // Chave
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoordId;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoordUTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoordESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoordESQ2;         // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoordIco;          // Deslocamentos para posicionamento dos ícones


   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstChave)  return(true);

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede) return(false);

   for (int i = 0; i < lstChave->Count; i++)
      {
      if ((xmlChave = lstChave->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto = xmlChave->Get_eqlig()) == NULL) return(false);
      if ((xmlId    = xmlEqpto->Get_id())             == NULL) return(false);

      str_chave.chv_id  = xmlId->Get_intId();
      str_chave.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 2) return(false);

      xmlCoordId                = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM               = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1              = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2              = xmlCoordId->Get_coordESQ2();
      str_chave.bar1_id         = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_chave.bar1_id         = CriaBarra(xmlCoordId);
      str_chave.esq.posbar1     = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordId                = lstCoordId->Get_Item(1);
      /*
      xmlCoordUTM               = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1              = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2              = xmlCoordId->Get_coordESQ2();
      str_chave.bar2_id         = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_chave.bar2_id          = CriaBarra(xmlCoordId);
      str_chave.esq.posbar2      = xmlCoordId->Get_dblPosBar();  // unck@20080807

      str_chave.codigo           = xmlId->Get_strCod();
      str_chave.extern_id        = xmlId->Get_strExtId();                        // Código externo. Inicialmente o mesmo código do chave
      str_chave.tipochave.id     = -1;
      str_chave.tipochave.tipo   = -1;
      str_chave.tipochave.codigo = xmlChave->Get_strTpoChv();
      str_chave.corn             = xmlChave->Get_dblCorNom();
      str_chave.estado           = xmlChave->Get_intEst();
      str_chave.esq.posbar3      = 0;
      str_chave.esq.comprimento  = 0;

      // Insere Chave em Monta
      //if ((eqpto = monta->InsereChave(str_chave)) == NULL) return(false);
      if ((eqpto = monta->InsereChave(str_chave)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicas(void)
   {
   //variáveis locais
   bool                    sucesso = true;
   _di_IXMLTCurvaTipicaList lstCurva;

   // Carrega as curvas típicas dos capacitores
   lstCurva = xmlConcessAux->Get_CurvaTipicaCapacitor();
   if (lstCurva)
      {
      sucesso = InsereConjuntoCurvasTipicasCapacitor(lstCurva);
      lstCurva->Clear();
      }
   // Carrega as curvas típicas das cargas
   if (sucesso)
      {
      lstCurva = xmlConcessAux->Get_CurvaTipicaCarga();
      if (lstCurva)
         {
         sucesso = InsereConjuntoCurvasTipicasCarga(lstCurva);
         lstCurva->Clear();
         }
      }
   // Carrega as curvas típicas das cargas de iluminação pública
   if (sucesso)
      {
      lstCurva = xmlConcessAux->Get_CurvaTipicaCargaIP();
      if (lstCurva)
         {
         sucesso = InsereConjuntoCurvasTipicasCargaIP(lstCurva);
         lstCurva->Clear();
         }
      }
   // Carrega as curvas típicas dos geradores fotovoltaicos
   if (sucesso)
      {
      lstCurva = xmlConcessAux->Get_CurvaTipicaGeradorFoto();
      if (lstCurva)
         {
         sucesso = InsereConjuntoCurvasTipicasGeradorFoto(lstCurva);
         lstCurva->Clear();
         }
      }
   // Carrega as curvas típicas dos reatores
   if (sucesso)
      {
      lstCurva = xmlConcessAux->Get_CurvaTipicaReator();
      if (lstCurva)
         {
         sucesso = InsereConjuntoCurvasTipicasReator(lstCurva);
         lstCurva->Clear();
         }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicasCarga(_di_IXMLTCurvaTipicaList lstCurva)
   {
   _di_IXMLTId          xmlId;
   _di_IXMLTCurvaTipica xmlCurva;
   VTCurva*             curva  = NULL;
   VTCurvas*            curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!lstCurva)                                               return(false);
   if (!curvas)                                                 return(false);
   // Loop da lista de curvas
   for (int i = 0; i < lstCurva->Count; i++)
      {
      // Evita duplicações
      if ((xmlCurva = lstCurva->Items[i]) == NULL)              return(false);
      if ((xmlId = xmlCurva->Get_id()) == NULL)                 return(false);
      if (curvas->ExisteCurvaTipicaCarga(xmlId->Get_intId()))   continue;
      // Cria a curva
      if ((curva = CriaCurvaTipica(xmlCurva, true)) == NULL)    return(false);
      // Insere a curva criada
      if (!curvas->InsereCurvaTipicaCarga(curva))               return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicasCargaIP(_di_IXMLTCurvaTipicaList lstCurva)
   {
   _di_IXMLTId          xmlId;
   _di_IXMLTCurvaTipica xmlCurva;
   VTCurva*             curva   = NULL;
   VTCurvas*            curvas  = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTClasses*           classes = (VTClasses*)apl->GetObject(__classid(VTClasses));

   // Evita explosões
   if (!lstCurva)                                            return(false);
   if (!curvas)                                              return(false);
   // Loop da lista de curvas
   for (int i = 0; i < lstCurva->Count; i++)
      {
      // Evita duplicações
      if ((xmlCurva = lstCurva->Items[i]) == NULL)           return(false);
      if ((xmlId = xmlCurva->Get_id()) == NULL)              return(false);
      if (curvas->ExisteCurvaTipicaIP(xmlId->Get_intId()))   continue;
      // Cria a curva
      if ((curva = CriaCurvaTipica(xmlCurva, false)) == NULL)       return(false);
      //proteção: verifica se a Curva está associada com a classe de consumidor IP
      if (curva->Classe == NULL)
         {//define Classe de consumidor
         curva->Classe = classes->ExisteClasseTag(consIP);
         }
      // Insere a curva criada
      if (!curvas->InsereCurvaTipicaIP(curva))               return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicasCapacitor(_di_IXMLTCurvaTipicaList lstCurva)
   {
   _di_IXMLTId          xmlId;
   _di_IXMLTCurvaTipica xmlCurva;
   VTCurva*             curva  = NULL;
   VTCurvas*            curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!lstCurva)                                                   return(false);
   if (!curvas)                                                     return(false);
   // Loop da lista de curvas
   for (int i = 0; i < lstCurva->Count; i++)
      {
      // Evita duplicações
      if ((xmlCurva = lstCurva->Items[i]) == NULL)                  return(false);
      if ((xmlId = xmlCurva->Get_id()) == NULL)                     return(false);
      if (curvas->ExisteCurvaTipicaCapacitor(xmlId->Get_intId()))   continue;
      // Cria a curva
      if ((curva = CriaCurvaTipica(xmlCurva)) == NULL)              return(false);
      // Insere a curva criada
      if (!curvas->InsereCurvaTipicaCapacitor(curva))               return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicasGeradorFoto(_di_IXMLTCurvaTipicaList lstCurva)
   {
   _di_IXMLTId          xmlId;
   _di_IXMLTCurvaTipica xmlCurva;
   VTCurva*             curva  = NULL;
   VTCurvas*            curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!lstCurva)                                                           return(false);
   if (!curvas)                                                             return(false);
   // Loop da lista de curvas
   for (int i = 0; i < lstCurva->Count; i++)
      {
      // Evita duplicações
      if ((xmlCurva = lstCurva->Items[i]) == NULL)                          return(false);
      if ((xmlId = xmlCurva->Get_id()) == NULL)                             return(false);
      if (curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(xmlId->Get_intId())) continue;
      // Cria a curva
      if ((curva = CriaCurvaTipica(xmlCurva)) == NULL)                      return(false);
      // Insere a curva criada
      if (!curvas->InsereCurvaTipicaGeradorFotovoltaico(curva))             return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoCurvasTipicasReator(_di_IXMLTCurvaTipicaList lstCurva)
   {
   _di_IXMLTId          xmlId;
   _di_IXMLTCurvaTipica xmlCurva;
   VTCurva*             curva  = NULL;
   VTCurvas*            curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!lstCurva)                                              return(false);
   if (!curvas)                                                return(false);
   // Loop da lista de curvas
   for (int i = 0; i < lstCurva->Count; i++)
      {
      // Evita duplicações
      if ((xmlCurva = lstCurva->Items[i]) == NULL)             return(false);
      if ((xmlId = xmlCurva->Get_id()) == NULL)                return(false);
      if (curvas->ExisteCurvaTipicaReator(xmlId->Get_intId())) continue;
      // Cria a curva
      if ((curva = CriaCurvaTipica(xmlCurva)) == NULL)         return(false);
      // Insere a curva criada
      if (!curvas->InsereCurvaTipicaReator(curva))             return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoFiltro(_di_IXMLTEqBarFiltroList lstFiltro, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strFILTRO                   str_filtro;     // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarFiltro        xmlFiltro;      // Filtro
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones
   _di_IXMLTZ                  xmlZ;           // Impedância

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstFiltro) return(true);

   for (int i = 0; i < lstFiltro->Count; i++)
      {
       if ((xmlFiltro = lstFiltro->Get_Item(i)) == NULL) return(false);
       if ((xmlEqpto  = xmlFiltro->Get_eqbar()) == NULL) return(false);
       if ((xmlId     = xmlEqpto->Get_id())     == NULL) return(false);

       str_filtro.id         = xmlId->Get_intId();
       str_filtro.codigo     = xmlId->Get_strCod();
       str_filtro.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

       lstCoordId = xmlEqpto->Get_coordId();
       // Verifica o número de coordenadas esperado.
       if (lstCoordId->Count < 1) return(false);

       xmlCoordId             = lstCoordId->Get_Item(0);
       /*
       xmlCoordUTM            = xmlCoordId->Get_coordUTM();
       xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
       xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
       str_filtro.bar_id      = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                          xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                          xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                          xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
       */
       str_filtro.bar_id      = CriaBarra(xmlCoordId);
       str_filtro.esq.posbar  = xmlCoordId->Get_dblPosBar();  // unc   k@20080807

       xmlCoordIco            = xmlCoordId->Get_coordIco();
       str_filtro.esq.dxbar   = xmlCoordIco->Get_dblDxBar();   // unck@20080807
       str_filtro.esq.dybar   = xmlCoordIco->Get_dblDyBar();   // unck@20080807

       str_filtro.vnom        = xmlFiltro->Get_dblTnsNom();
       str_filtro.snom        = xmlFiltro->Get_dblPotNom();

       xmlZ                   = xmlFiltro->Get_z0();
       str_filtro.z0.r        = xmlZ->Get_dblR();
       str_filtro.z0.x        = xmlZ->Get_dblX();

       // Insere filtro em Monta
       //if ((eqpto = monta->InsereFiltro(str_filtro)) == NULL) return(false);
       if ((eqpto = monta->InsereFiltro(str_filtro)) == NULL) continue;

       // Insere o conjunto de medições
       if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoGerador(_di_IXMLTEqBarGeradorList lstGerador, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strGERADOR                  str_gerador;    // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarGerador       xmlGerador;     // Carga (Típica ou Especial)
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones
   _di_IXMLTPatamPropList       lstPatamar;     // Lista de patamares
   _di_IXMLTPatamProp          xmlPatamar;     // Lista de patamares
   _di_IXMLTHorMin             xmlhm_ini;      // Hora e Minuto inicial do patamar
   _di_IXMLTHorMin             xmlhm_fim;      // Hora e Minuto final   do patamar
   _di_IXMLTZseq               xmlZSeq;        // Impedância de seqüência
   _di_IXMLTZ                  xmlZ;           // Impedância

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstGerador) return(true);

   for (int i = 0; i < lstGerador->Count; i++)
      {
      if ((xmlGerador = lstGerador->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto   = xmlGerador->Get_eqbar()) == NULL) return(false);
      if ((xmlId      = xmlEqpto->Get_id())      == NULL) return(false);

      str_gerador.ger_id     = xmlId->Get_intId();
      str_gerador.codigo     = xmlId->Get_strCod();
      str_gerador.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 1) return(false);

      xmlCoordId              = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM             = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1            = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2            = xmlCoordId->Get_coordESQ2();
      str_gerador.bar_id      = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                          xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                          xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                          xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_gerador.bar_id      = CriaBarra(xmlCoordId);
      str_gerador.esq.posbar  = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordIco             = xmlCoordId->Get_coordIco();
      str_gerador.esq.dxbar   = xmlCoordIco->Get_dblDxBar();   // unck@20080807
      str_gerador.esq.dybar   = xmlCoordIco->Get_dblDyBar();   // unck@20080807

      str_gerador.tiposup      = xmlGerador->Get_intTpoSup();
      str_gerador.tipo_geracao = gerVAPOR;
      str_gerador.ligacao      = xmlGerador->Get_intTpoLig();
      str_gerador.vnom         = xmlGerador->Get_dblTnsNom();
      str_gerador.snom_mva     = xmlGerador->Get_dblPotNom();
      str_gerador.smax_mva     = xmlGerador->Get_dblPotNom();
      str_gerador.fplim        = xmlGerador->Get_dblFtrPotLim();

      xmlZSeq                 = xmlGerador->Get_zSeq();
      xmlZ                    = xmlZSeq->Get_z0();
      str_gerador.z0.r        = xmlZ->Get_dblR();
      str_gerador.z0.x        = xmlZ->Get_dblX();
      xmlZ                    = xmlZSeq->Get_z1();
      str_gerador.z1.r        = xmlZ->Get_dblR();
      str_gerador.z1.x        = xmlZ->Get_dblX();

      // Carrega os patamares
      if ((lstPatamar = xmlGerador->Get_Patamar()) != NULL)
         {
         str_gerador.curva.id        = -1;
         str_gerador.curva.tipica    = false;
         str_gerador.curva.num_pat   = 0;
         str_gerador.curva.num_valor = nvCURVA_PQVT;
         for (int j = 0; j < lstPatamar->Count; j++)
            {
            xmlPatamar = lstPatamar->Get_Item(j);
            str_gerador.curva.num_pat++;
            xmlhm_ini = xmlPatamar->Get_horIni();
            str_gerador.curva.patamar[j].hm_ini.hora   = xmlhm_ini->Get_intHor();
            str_gerador.curva.patamar[j].hm_ini.minuto = xmlhm_ini->Get_intMin();
            xmlhm_fim = xmlPatamar->Get_horFim();
            str_gerador.curva.patamar[j].hm_fim.hora   = xmlhm_fim->Get_intHor();
            str_gerador.curva.patamar[j].hm_fim.minuto = xmlhm_fim->Get_intMin();
            str_gerador.curva.patamar[j].valor[indP] = xmlPatamar->Get_dblPotAtv() / 1000.; //kW   => MW
            str_gerador.curva.patamar[j].valor[indQ] = xmlPatamar->Get_dblPotRtv() / 1000.; //kVAr => MVAr
            str_gerador.curva.patamar[j].valor[indV] = xmlPatamar->Get_dblMdlTns();
            str_gerador.curva.patamar[j].valor[indT] = xmlPatamar->Get_dblAng();
            }
         }
      //insere Gerador em Monta
      if ((eqpto = monta->InsereGerador(str_gerador)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoMedidor(_di_IXMLTEqBarMedidorList lstMedidor, VTRede* rede)
   {
   VTEqpto                     *eqpto;
   strMEDIDOR                  str_medidor;    // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarMedidor       xmlMedidor;     // Medidor
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         cnjCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstMedidor) return(true);

   for (int i = 0; i < lstMedidor->Count; i++)
      {
      if ((xmlMedidor = lstMedidor->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto   = xmlMedidor->Get_eqbar()) == NULL) return(false);
      if ((xmlId      = xmlEqpto->Get_id())      == NULL) return(false);

      str_medidor.medidor_id = xmlId->Get_intId();
      str_medidor.codigo     = xmlId->Get_strCod();
      str_medidor.extern_id  = xmlId->Get_strExtId();
      cnjCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (cnjCoordId->Count != 1) return(false);

      xmlCoordId             = cnjCoordId->Get_Item(0);
      /*
      xmlCoordUTM            = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
      str_medidor.bar_id     = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                         xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                         xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                         xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_medidor.bar_id     = CriaBarra(xmlCoordId);
      str_medidor.esq.posbar = xmlCoordId->Get_dblPosBar();

      xmlCoordIco            = xmlCoordId->Get_coordIco();
      str_medidor.esq.dxbar  = xmlCoordIco->Get_dblDxBar();
      str_medidor.esq.dybar  = xmlCoordIco->Get_dblDyBar();

      //insere Medidor em Monta
      if ((eqpto = monta->InsereMedidor(str_medidor)) == NULL) continue;
      // Insere o conjunto de medições de tensão da Barra do Medidor
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   } // InsereConjuntoMedidor

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoMutua(_di_IXMLTMutuaList lstMutua)
   {
   strMUTUA             str_mutua;    // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTMutua       xmlMutua;     // Mutua
   _di_IXMLTMutuaTreList cnjMutuaTre;  // Trechos da mútua
   _di_IXMLTMutuaTre    xmlMutuaTre;  // Trecho da mútua
   _di_IXMLTId          xmlId;        // Id do equipamento
   _di_IXMLTZseq        xmlZSeq;      // Impedância de seqüência
   _di_IXMLTZ           xmlZ;         // Impedância

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstMutua) return(true);

   for (int i = 0; i < lstMutua->Count; i++)
      {
      if ((xmlMutua = lstMutua->Get_Item(i)) == NULL) return(false);
      if ((xmlId    = xmlMutua->Get_id())    == NULL) return(false);

      str_mutua.id           = xmlId->Get_intId();
      str_mutua.codigo       = xmlId->Get_strCod();
      str_mutua.extern_id    = xmlId->Get_strExtId();
      str_mutua.rede_id      = xmlMutua->Get_intRedeId();

      cnjMutuaTre            = xmlMutua->Get_MutuaTre();

      xmlMutuaTre            = cnjMutuaTre->Get_Item(0);
      str_mutua.tre1_id      = xmlMutuaTre->Get_intId();
      str_mutua.tre1_bar_id  = xmlMutuaTre->Get_intSentido();
      str_mutua.postre1      = xmlMutuaTre->Get_dblPos();

      xmlMutuaTre            = cnjMutuaTre->Get_Item(1);
      str_mutua.tre2_id      = xmlMutuaTre->Get_intId();
      str_mutua.tre2_bar_id  = xmlMutuaTre->Get_intSentido();
      str_mutua.postre2      = xmlMutuaTre->Get_dblPos();

      xmlZSeq               = xmlMutua->Get_zSeq();
      xmlZ                  = xmlZSeq->Get_z0();
      str_mutua.z0.r        = xmlZ->Get_dblR();
      str_mutua.z0.x        = xmlZ->Get_dblX();
      xmlZ                  = xmlZSeq->Get_z1();
      str_mutua.z1.r        = xmlZ->Get_dblR();
      str_mutua.z1.x        = xmlZ->Get_dblX();

      //insere Mutua em Monta
      if (monta->InsereMutua(str_mutua) == NULL) continue;

      //destrói objetos XML
      //delete xmlMutua;
      //delete cnjMutuaTre;
      //delete xmlMutuaTre;
      //delete xmlId;      
      //delete xmlZSeq;    
      //delete xmlZ;       
      }
    return(true);
    } // InsereConjuntoMutua

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoReator(_di_IXMLTEqBarReatorList lstReator, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strREATOR                   str_reator;     // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarReator        xmlReator;      // Reator
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstReator) return(true);

   for (int i = 0; i < lstReator->Count; i++)
      {
      if ((xmlReator = lstReator->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto  = xmlReator->Get_eqbar()) == NULL) return(false);
      if ((xmlId     = xmlEqpto->Get_id())     == NULL) return(false);

      str_reator.id         = xmlId->Get_intId();
      str_reator.codigo     = xmlId->Get_strCod();
      str_reator.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 1) return(false);

      xmlCoordId             = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM            = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
      str_reator.bar_id      = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                         xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                         xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                         xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_reator.bar_id      = CriaBarra(xmlCoordId);
      str_reator.esq.posbar  = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordIco            = xmlCoordId->Get_coordIco();
      str_reator.esq.dxbar   = xmlCoordIco->Get_dblDxBar();   // unck@20080807
      str_reator.esq.dybar   = xmlCoordIco->Get_dblDyBar();   // unck@20080807

      str_reator.ligacao     = xmlReator->Get_intTpoLig();
      str_reator.vnom        = xmlReator->Get_dblTnsNom();
      str_reator.q_mvar      = xmlReator->Get_dblPotRtv();

      //insere Reator em Monta
      if ((eqpto = monta->InsereReator(str_reator)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
       }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoRede(_di_IXMLTRedeList lstRede)
   {
   //variáveis locais
   _di_IXMLTRede          xmlRede;
   _di_IXMLTCnjEqBar      lstEqBar;
   _di_IXMLTCnjEqLig      lstEqLig;
   VTRede*                rede;

   //insere Rede
   for (int n = 0; n < lstRede->Count; n++)
      {                                                   
      if ((xmlRede = lstRede->Items[n]) != NULL)
         {
         if ((rede = InsereRede(xmlRede)) == NULL) return(false);
         //inicia controle de inserção de Barra
         monta->InsereBarraStart();
         //insere Eqbar e respectiva Barra da Rede
         lstEqBar = xmlRede->Get_cnjEqBar();
         if (lstEqBar)
            { // Encontrou um EqBar (ini)
            // A carga dos medidores é prioritária, pois os outros equipamentos dependem
            if (! InsereConjuntoMedidor(lstEqBar->Get_Medidor(), rede))       return(false);
            lstEqBar->Get_Medidor()->Clear();

            if (! InsereConjuntoCapacitor(lstEqBar->Get_Capacitor(), rede))   return(false);
            lstEqBar->Get_Capacitor()->Clear();

            if (! InsereConjuntoCarga(lstEqBar->Get_Carga(), rede))           return(false);
            lstEqBar->Get_Carga()->Clear();

            if (! InsereConjuntoCargaIP(lstEqBar->Get_CargaIP(), rede))       return(false);
            lstEqBar->Get_CargaIP()->Clear();

            if (! InsereConjuntoFiltro(lstEqBar->Get_Filtro(), rede))         return(false);
            lstEqBar->Get_Filtro()->Clear();

            if (! InsereConjuntoGerador(lstEqBar->Get_Gerador(), rede))       return(false);
            lstEqBar->Get_Gerador()->Clear();

            if (! InsereConjuntoReator(lstEqBar->Get_Reator(), rede))         return(false);
            lstEqBar->Get_Reator()->Clear();

            if (! InsereConjuntoSuprimento(lstEqBar->Get_Suprimento(), rede)) return(false);
            lstEqBar->Get_Suprimento()->Clear();

            if (! InsereConjuntoTrafoZZ(lstEqBar->Get_TrafoZZ(), rede))       return(false);
            lstEqBar->Get_TrafoZZ()->Clear();
            } // Encontrou um EqBar (fim)

         //insere Ligacao e respectivas Barras da Rede
         lstEqLig = xmlRede->Get_cnjEqLig();
         if (lstEqLig)
            { // Encontrou um lstEqLig (ini)
            if (! InsereConjuntoCapserie(lstEqLig->Get_CapacitorSerie(), rede))    return(false);
            lstEqLig->Get_CapacitorSerie()->Clear();

            if (! InsereConjuntoChave(lstEqLig->Get_Chave(), rede))                return(false);
            lstEqLig->Get_Chave()->Clear();

            if (! InsereConjuntoRegulador(lstEqLig->Get_Regulador(), rede))        return(false);
            lstEqLig->Get_Regulador()->Clear();

            if (! InsereConjuntoTrafo(lstEqLig->Get_Trafo(), rede))                return(false);
            lstEqLig->Get_Trafo()->Clear();

            if (! InsereConjuntoTrafo3E(lstEqLig->Get_Trafo3E(), rede))            return(false);
            lstEqLig->Get_Trafo3E()->Clear();

            if (! InsereConjuntoTrecho(lstEqLig->Get_Trecho(), rede))              return(false);
            lstEqLig->Get_Trecho()->Clear();

            } // Encontrou um lstEqLig (fim)
         //insere o conjunto de mútuas
         if (! InsereConjuntoMutua(xmlRede->Get_Mutua())) return(false);
         xmlRede->Get_Mutua()->Clear();
         //finaliza controle de inserção de Barra
         monta->InsereBarraStop();

         // unck@20100527
         // ATENÇÃO Guaraldo: limpeza geral de memória. Libera o espaço ocupado pela rede
         // Isso parte do princípio que uma rede sempre cabe inteira na memória
         // Se houver uma situação em que não couber, essa limpeza precisa passar para
         // dentro da rede, eliminando listas parciais de EqBar e EqLig
         xmlRede->ChildNodes->Clear();
         xmlRede = NULL;

         // Rede válida (fim)
         }
      //destrói objetos XML
      //delete xmlRede;
      //delete lstEqBar;
      //delete lstEqLig;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoRegulador(_di_IXMLTEqLigReguladorList lstRegulador, VTRede* rede)
   {
   VTEqpto*                      eqpto;
   strREGULADOR                  str_regulador;        // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigRegulador       xmlRegulador;         // Regulador
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoord1_Id;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoord1_UTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoord1_ESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoord1_ESQ2;         // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord1_Ico;          // Deslocamentos para posicionamento dos ícones
   _di_IXMLTCoordId              xmlCoord2_Id;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoord2_UTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoord2_ESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoord2_ESQ2;         // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord2_Ico;          // Deslocamentos para posicionamento dos ícones
   _di_IXMLTCoordId              xmlCoord3_Id;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoord3_UTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoord3_ESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoord3_ESQ2;         // Coordenada Esquema 2
   _di_IXMLTZseq                 xmlZSeq;              // Impedância de seqüência
   _di_IXMLTZ                    xmlZ;                 // Impedância
   _di_IXMLTRegTapFix            xmlRegTapFix;         // Estrutura que armazena dados específicos de cada tipo de regulador
   _di_IXMLTRegTapAjs            xmlRegTapAjs;         // Estrutura que armazena dados específicos de cada tipo de regulador

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstRegulador) return(true);

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede) return(false);

   for (int i = 0; i < lstRegulador->Count; i++)
      {
      if ((xmlRegulador = lstRegulador->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto     = xmlRegulador->Get_eqlig()) == NULL) return(false);
      if ((xmlId        = xmlEqpto->Get_id())        == NULL) return(false);

      str_regulador.id      = xmlId->Get_intId();
      str_regulador.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 2) return(false);

      xmlCoord1_Id               = lstCoordId->Get_Item(0);
      /*
      xmlCoord1_UTM              = xmlCoord1_Id->Get_coordUTM();
      xmlCoord1_ESQ1             = xmlCoord1_Id->Get_coordESQ1();
      xmlCoord1_ESQ2             = xmlCoord1_Id->Get_coordESQ2();
      str_regulador.bar1_id      = CriaBarra(xmlCoord1_Id->Get_intCoordNdx(), xmlCoord1_Id->Get_strCod(),
                                             xmlCoord1_UTM->Get_dblX(),       xmlCoord1_UTM->Get_dblY(),
                                             xmlCoord1_ESQ1->Get_dblX(),      xmlCoord1_ESQ1->Get_dblY(),
                                             xmlCoord1_ESQ2->Get_dblX(),      xmlCoord1_ESQ2->Get_dblY());
      */
      str_regulador.bar1_id      = CriaBarra(xmlCoord1_Id);
      str_regulador.esq.posbar1  = xmlCoord1_Id->Get_dblPosBar();  // unck@20080807

      xmlCoord2_Id               = lstCoordId->Get_Item(1);
      /*
      xmlCoord2_UTM              = xmlCoord2_Id->Get_coordUTM();
      xmlCoord2_ESQ1             = xmlCoord2_Id->Get_coordESQ1();
      xmlCoord2_ESQ2             = xmlCoord2_Id->Get_coordESQ2();
      str_regulador.bar2_id      = CriaBarra(xmlCoord2_Id->Get_intCoordNdx(), xmlCoord2_Id->Get_strCod(),
                                             xmlCoord2_UTM->Get_dblX(),       xmlCoord2_UTM->Get_dblY(),
                                             xmlCoord2_ESQ1->Get_dblX(),      xmlCoord2_ESQ1->Get_dblY(),
                                             xmlCoord2_ESQ2->Get_dblX(),      xmlCoord2_ESQ2->Get_dblY());
      */
      str_regulador.bar2_id      = CriaBarra(xmlCoord2_Id);
      str_regulador.esq.posbar2  = xmlCoord2_Id->Get_dblPosBar();  // unck@20080807

      str_regulador.codigo              = xmlId->Get_strCod();
      str_regulador.extern_id           = xmlId->Get_strExtId();                    // Código externo. Inicialmente o mesmo código do regulador
      str_regulador.by_pass             = xmlRegulador->Get_bolBypass();
      str_regulador.ligacao             = xmlRegulador->Get_intTpoLig();
      str_regulador.snom_mva            = xmlRegulador->Get_dblPotNom();
      str_regulador.var_tensao          = xmlRegulador->Get_dblFxaVar();
      str_regulador.num_passo           = xmlRegulador->Get_intPasNum();
      //verifica se fluxo reversível
      //str_regulador.sentido.reversivel  = xmlRegulador->Get_bolSntRev();
      str_regulador.sentido.bar_ref_id  = -1;
      /*bloqueado
      //se fluxo não reversível, determina barra de refeência do sentido do fluxo
      xmlCoordId                        = xmlRegulador->Get_coordBarRefSnt();
      xmlCoordUTM                       = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1                      = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2                      = xmlCoordId->Get_coordESQ2();
      str_regulador.sentido.bar_rev_id  = CriaBarra(xmlCoordId);
      */
      //tap fixo: define valores default
      str_regulador.cfg_fixo.passo      = 0;
      //tap variável: define valores default
      str_regulador.cfg_auto.bar_ref_id = 0;
      str_regulador.cfg_auto.v_pu       = 1;
      //tap fixo ou variável
      xmlRegTapAjs = xmlRegulador->Get_regTapAjs();
      xmlRegTapFix = xmlRegulador->Get_regTapFix();
      if (xmlRegTapFix->HasChildNodes)
          {// Tap Fixo (ini)
          //str_regulador.ajuste_auto     = false;
          str_regulador.cfg_fixo.passo  = xmlRegTapFix->Get_intPasSel();
          // Tap Fixo (fim)
          }
      else if (xmlRegTapAjs->HasChildNodes)
          {
          // Tap Ajustável (ini)
          //str_regulador.ajuste_auto         = true;
          xmlCoord3_Id                      = xmlRegTapAjs->Get_coordBarRefCfg();
          /*
          xmlCoord3_UTM                     = xmlCoord3_Id->Get_coordUTM();
          xmlCoord3_ESQ1                    = xmlCoord3_Id->Get_coordESQ1();
          xmlCoord3_ESQ2                    = xmlCoord3_Id->Get_coordESQ2();
          str_regulador.cfg_auto.bar_ref_id = CriaBarra(xmlCoord3_Id->Get_intCoordNdx(), xmlCoord3_Id->Get_strCod(),
                                                        xmlCoord3_UTM->Get_dblX(),       xmlCoord3_UTM->Get_dblY(),
                                                        xmlCoord3_ESQ1->Get_dblX(),      xmlCoord3_ESQ1->Get_dblY(),
                                                        xmlCoord3_ESQ2->Get_dblX(),      xmlCoord3_ESQ2->Get_dblY());
          */
          str_regulador.cfg_auto.bar_ref_id = CriaBarra(xmlCoord3_Id);
          str_regulador.cfg_auto.v_pu       = xmlRegTapAjs->Get_dblTnsAlvo();
          // Tap Ajustável (fim)
          }
      else
          {
          return(false);
          }
      // Na versão 2.0, a impedância estava comentada.
      xmlZSeq                       = xmlRegulador->Get_zSeq();
      xmlZ                          = xmlZSeq->Get_z0();
      str_regulador.z0.r            = xmlZ->Get_dblR();
      str_regulador.z0.x            = xmlZ->Get_dblX();
      xmlZ                          = xmlZSeq->Get_z1();
      str_regulador.z1.r            = xmlZ->Get_dblR();
      str_regulador.z1.x            = xmlZ->Get_dblX();

      str_regulador.esq.posbar3     = 0;
      str_regulador.esq.comprimento = 0;

      //Insere o regulador em monta
      if ((eqpto = monta->InsereRegulador(str_regulador)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;

      //destrói objetos XML
      //delete xmlRegulador;
      //delete xmlEqpto;      
      //delete xmlId;         
      //delete lstCoordId;    
      //delete xmlCoord1_Id;  
      //delete xmlCoord1_UTM; 
      //delete xmlCoord1_ESQ1;
      //delete xmlCoord1_ESQ2;
      //delete xmlCoord1_Ico; 
      //delete xmlCoord2_Id;  
      //delete xmlCoord2_UTM; 
      //delete xmlCoord2_ESQ1;
      //delete xmlCoord2_ESQ2;
      //delete xmlCoord2_Ico; 
      //delete xmlCoord3_Id;  
      //delete xmlCoord3_UTM; 
      //delete xmlCoord3_ESQ1;
      //delete xmlCoord3_ESQ2;
      //delete xmlZSeq;       
      //delete xmlZ;          
      //delete xmlRegTapFix;  
      //delete xmlRegTapAjs;  
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoSuporte(_di_IXMLTSuporteList lstSuporte)
   {
   //variáveis locais
   AnsiString                text;
   _di_IXMLTSuporte          xmlSuporte;
   _di_IXMLTId               xmlId;;
   _di_IXMLTSuportePtoFixList xmlPtoFixList;
   _di_IXMLTSuportePtoFix    xmlPtoFix;
   _di_IXMLTCoord            xmlCoord;
   strSUPORTE                str_suporte;    // Estrutura de armazenamento a ser inserida em TMonta

   //proteção: verifica o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (! lstSuporte) return(true);

   //loop p/ conjunto de Cabos
   for (int i = 0; i < lstSuporte->Count; i++)
      {
      if ((xmlSuporte   = lstSuporte->Get_Item(i))  == NULL) return(false);
      if ((xmlId        = xmlSuporte->Get_id())     == NULL) return(false);
      if ((xmlPtoFixList = xmlSuporte->Get_PtoFix()) == NULL) return(false);
      try{//lê dados do Suporte
         str_suporte.id         = xmlId->Get_intId();
         str_suporte.codigo     = xmlId->Get_strCod();
         str_suporte.status     = sttNOVO;
         //tipo de suporte
         text                   = xmlSuporte->Get_strTpo();
         if      (text.AnsiCompareIC("aéreo")       == 0) str_suporte.tipo = suporteAEREO;
         else if (text.AnsiCompareIC("subterrâneo") == 0) str_suporte.tipo = suporteSUBTERRANEO;
         else if (text.AnsiCompareIC("submerso")    == 0) str_suporte.tipo = suporteSUBMERSO;
         else                                             str_suporte.tipo = suporteAEREO;
         //tensões mínima e máxima
         str_suporte.vmin       = xmlSuporte->Get_dblTnsMin();
         str_suporte.vmax       = xmlSuporte->Get_dblTnsMax();
         //determina conjunto de pontos de fixação
         str_suporte.num_pto = xmlPtoFixList->GetCount();
         for (int i = 0; i < str_suporte.num_pto; i++)
            {
            xmlPtoFix = xmlPtoFixList->Get_Item(i);
            xmlCoord = xmlPtoFix->Get_coordPnt();
            str_suporte.pto_fixacao[i].id      = xmlPtoFix->Get_intId();
            str_suporte.pto_fixacao[i].codigo  = "Ponto " + IntToStr(i+1);
            str_suporte.pto_fixacao[i].coord_x = xmlCoord->Get_dblX();
            str_suporte.pto_fixacao[i].coord_y = xmlCoord->Get_dblY();
            }
         //insere suporte
         monta->InsereSuporte(str_suporte);
         }catch(Exception &e)
            {
            MsgLog("Erro na leitura de dados do Suporte " + IntToStr(i+1));
            continue;
            }
      //destrói objetos XML
      //delete xmlSuporte;
      //delete xmlId;;
      //delete xmlCoordList;
      //delete xmlCoord;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoSuprimento(_di_IXMLTEqBarSuprimentoList lstSuprimento, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strSUPRIMENTO               str_suprimento; // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarSuprimento    xmlSuprimento;  // Suprimento
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones
   _di_IXMLTPatamPropList       lstPatamar;     // Lista de patamares
   _di_IXMLTPatamProp          xmlPatamar;     // Lista de patamares
   _di_IXMLTHorMin             xmlhm_ini;      // Hora e Minuto inicial do patamar
   _di_IXMLTHorMin             xmlhm_fim;      // Hora e Minuto final   do patamar
   _di_IXMLTZseq               xmlZSeq;        // Impedância de seqüência
   _di_IXMLTZ                  xmlZ0;          // Impedância
   _di_IXMLTZ                  xmlZ1;          // Impedância
   _di_IXMLTPotCpx             xmlPotC3f;      // Potência de curto 3f
   _di_IXMLTPotCpx             xmlPotCft;      // Potência de curto ft

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstSuprimento) return(true);
   for (int i = 0; i < lstSuprimento->Count; i++)
      {
      if ((xmlSuprimento = lstSuprimento->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto      = xmlSuprimento->Get_eqbar()) == NULL) return(false);
      if ((xmlId         = xmlEqpto->Get_id())         == NULL) return(false);

      str_suprimento.sup_id     = xmlId->Get_intId();
      str_suprimento.codigo     = xmlId->Get_strCod();
      str_suprimento.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 1) return(false);

      xmlCoordId                 = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM                = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1               = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2               = xmlCoordId->Get_coordESQ2();
      str_suprimento.bar_id      = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                             xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                             xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                             xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_suprimento.bar_id      = CriaBarra(xmlCoordId);
      str_suprimento.esq.posbar  = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordIco                = xmlCoordId->Get_coordIco();
      str_suprimento.esq.dxbar   = xmlCoordIco->Get_dblDxBar();   // unck@20080807
      str_suprimento.esq.dybar   = xmlCoordIco->Get_dblDyBar();   // unck@20080807

      str_suprimento.tiposup     = xmlSuprimento->Get_intTpoSup();
      str_suprimento.smax        = xmlSuprimento->Get_dblPotMax();
      str_suprimento.vnom_kv     = xmlSuprimento->Get_dblTnsNom();

      xmlZSeq                    = xmlSuprimento->Get_zEqv();
      xmlZ0                      = xmlZSeq->Get_z0();
      str_suprimento.zeq0.r      = xmlZ0->Get_dblR();
      str_suprimento.zeq0.x      = xmlZ0->Get_dblX();
      xmlZ1                      = xmlZSeq->Get_z1();
      str_suprimento.zeq1.r      = xmlZ1->Get_dblR();
      str_suprimento.zeq1.x      = xmlZ1->Get_dblX();

      xmlPotC3f                  = xmlSuprimento->Get_cpxPotCC3f();
      str_suprimento.pcc_3f.p    = xmlPotC3f->Get_dblPotAtv();
      str_suprimento.pcc_3f.q    = xmlPotC3f->Get_dblPotRtv();
      xmlPotCft                  = xmlSuprimento->Get_cpxPotCCFt();
      str_suprimento.pcc_ft.p    = xmlPotCft->Get_dblPotAtv();
      str_suprimento.pcc_ft.q    = xmlPotCft->Get_dblPotRtv();

      // No suprimento, a lista de patamares é obrigatória
      if ((lstPatamar = xmlSuprimento->Get_Patamar()) == NULL) return(false);
      //monta curva
      str_suprimento.curva.id        = -1;
      str_suprimento.curva.tipica    = false;
      str_suprimento.curva.num_pat   = 0;
      str_suprimento.curva.num_valor = nvCURVA_PQVT;
      for (int j = 0; j < lstPatamar->Count; j++)
          {
          xmlPatamar = lstPatamar->Get_Item(j);
          str_suprimento.curva.num_pat++;
          xmlhm_ini = xmlPatamar->Get_horIni();
          str_suprimento.curva.patamar[j].hm_ini.hora   = xmlhm_ini->Get_intHor();
          str_suprimento.curva.patamar[j].hm_ini.minuto = xmlhm_ini->Get_intMin();
          xmlhm_fim = xmlPatamar->Get_horFim();
          str_suprimento.curva.patamar[j].hm_fim.hora   = xmlhm_fim->Get_intHor();
          str_suprimento.curva.patamar[j].hm_fim.minuto = xmlhm_fim->Get_intMin();
          str_suprimento.curva.patamar[j].valor[indP] = xmlPatamar->Get_dblPotAtv() / 1000.; //kW   => MW
          str_suprimento.curva.patamar[j].valor[indQ] = xmlPatamar->Get_dblPotRtv() / 1000.; //kVAr => MVAr
          str_suprimento.curva.patamar[j].valor[indV] = xmlPatamar->Get_dblMdlTns();
          str_suprimento.curva.patamar[j].valor[indT] = xmlPatamar->Get_dblAng();
         }
      // Insere suprimento em Monta
      if ((eqpto = monta->InsereSuprimento(str_suprimento)) == NULL) continue;

      // Insere o conjunto de medições
       if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoTrafo(_di_IXMLTEqLigTrafoList lstTrafo, VTRede* rede)
   {
   VTEqpto*                      eqpto;
   strTRAFO                      str_trafo;            // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigTrafo           xmlTrafo;             // Trafo
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoord1_Id;         // Coordenada identificada (Barras)
   _di_IXMLTCoord                xmlCoord1_UTM;        // Coordenada UTM
   _di_IXMLTCoord                xmlCoord1_ESQ1;       // Coordenada Esquema 1
   _di_IXMLTCoord                xmlCoord1_ESQ2;       // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord1_Ico;        // Deslocamentos para posicionamento dos ícones
   _di_IXMLTCoordId              xmlCoord2_Id;         // Coordenada identificada (Barras)
   _di_IXMLTCoord                xmlCoord2_UTM;        // Coordenada UTM
   _di_IXMLTCoord                xmlCoord2_ESQ1;       // Coordenada Esquema 1
   _di_IXMLTCoord                xmlCoord2_ESQ2;       // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoord2_Ico;        // Deslocamentos para posicionamento dos ícones
   _di_IXMLTCoordId              xmlCoordLTC_Id;       // Coordenada identificada (Barras)
   _di_IXMLTCoord                xmlCoordLTC_UTM;      // Coordenada UTM
   _di_IXMLTCoord                xmlCoordLTC_ESQ1;     // Coordenada Esquema 1
   _di_IXMLTCoord                xmlCoordLTC_ESQ2;     // Coordenada Esquema 2
   _di_IXMLTZseq                 xmlZSeq;              // Impedância de seqüência
   _di_IXMLTZ                    xmlZ0;                // Impedância
   _di_IXMLTZ                    xmlZ1;                // Impedância
   _di_IXMLTZ                    xmlZat_pri;           // Impedância
   _di_IXMLTZ                    xmlZat_sec;           // Impedância
   _di_IXMLTLtc                  xmlLtc;               // LTC
   _di_IXMLTEnrolList             lstEnrol;             // Enrolamentos
   _di_IXMLTEnrol                xmlEnrol_pri;         // Enrolamento
   _di_IXMLTEnrol                xmlEnrol_sec;         // Enrolamento

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstTrafo) return(true);

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede) return(false);

   for (int i = 0; i < lstTrafo->Count; i++)
      {
      if ((xmlTrafo = lstTrafo->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto = xmlTrafo->Get_eqlig()) == NULL) return(false);
      if ((xmlId    = xmlEqpto->Get_id())    == NULL) return(false);

      str_trafo.trf_id  = xmlId->Get_intId();
      str_trafo.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 2) return(false);

      xmlCoord1_Id              = lstCoordId->Get_Item(0);
      xmlCoord1_UTM             = xmlCoord1_Id->Get_coordUTM();
      xmlCoord1_ESQ1            = xmlCoord1_Id->Get_coordESQ1();
      xmlCoord1_ESQ2            = xmlCoord1_Id->Get_coordESQ2();
      /*
      str_trafo.bar1_id         = CriaBarra(xmlCoord1_Id->Get_intCoordNdx(), xmlCoord1_Id->Get_strCod(),
                                            xmlCoord1_UTM->Get_dblX(),       xmlCoord1_UTM->Get_dblY(),
                                            xmlCoord1_ESQ1->Get_dblX(),      xmlCoord1_ESQ1->Get_dblY(),
                                            xmlCoord1_ESQ2->Get_dblX(),      xmlCoord1_ESQ2->Get_dblY());
      */
      str_trafo.bar1_id         = CriaBarra(xmlCoord1_Id);
      str_trafo.esq.posbar1     = xmlCoord1_Id->Get_dblPosBar();  // unck@20080807

      xmlCoord2_Id              = lstCoordId->Get_Item(1);
      xmlCoord2_UTM             = xmlCoord2_Id->Get_coordUTM();
      xmlCoord2_ESQ1            = xmlCoord2_Id->Get_coordESQ1();
      xmlCoord2_ESQ2            = xmlCoord2_Id->Get_coordESQ2();
      /*
      str_trafo.bar2_id         = CriaBarra(xmlCoord2_Id->Get_intCoordNdx(), xmlCoord2_Id->Get_strCod(),
                                            xmlCoord2_UTM->Get_dblX(),       xmlCoord2_UTM->Get_dblY(),
                                            xmlCoord2_ESQ1->Get_dblX(),      xmlCoord2_ESQ1->Get_dblY(),
                                            xmlCoord2_ESQ2->Get_dblX(),      xmlCoord2_ESQ2->Get_dblY());
      */
      str_trafo.bar2_id         = CriaBarra(xmlCoord2_Id);
      str_trafo.esq.posbar2     = xmlCoord2_Id->Get_dblPosBar();  // unck@20080807

      str_trafo.codigo          = xmlId->Get_strCod();
      str_trafo.extern_id       = xmlId->Get_strExtId();                        // Código externo. Inicialmente o mesmo código do trafo
      str_trafo.auto_trafo      = xmlTrafo->Get_bolAuto();
      str_trafo.snom_mva        = xmlTrafo->Get_dblPotNom();

      lstEnrol = xmlTrafo->Get_Enrol();
      // Verifica o número de enrolamentos esperado.
      if (lstEnrol->Count != 2) return(false);

      xmlEnrol_pri = lstEnrol->Get_Item(0);
      str_trafo.pri.ligacao     = xmlEnrol_pri->Get_intTpoLig();
      str_trafo.pri.vnom        = xmlEnrol_pri->Get_dblTnsNom();
      str_trafo.pri.tap         = xmlEnrol_pri->Get_dblTnsTap();
      str_trafo.pri.defasagem   = xmlEnrol_pri->Get_dblAngDef();
      str_trafo.pri.fases       = fases->ExisteFase(xmlEnrol_pri->Get_strFases());
      xmlZat_pri                = xmlEnrol_pri->Get_zTerra();
      str_trafo.pri.zterra.r    = xmlZat_pri->Get_dblR();
      str_trafo.pri.zterra.x    = xmlZat_pri->Get_dblX();

      xmlEnrol_sec = lstEnrol->Get_Item(1);
      str_trafo.sec.ligacao     = xmlEnrol_sec->Get_intTpoLig();
      str_trafo.sec.vnom        = xmlEnrol_sec->Get_dblTnsNom();
      str_trafo.sec.tap         = xmlEnrol_sec->Get_dblTnsTap();
      str_trafo.sec.defasagem   = xmlEnrol_sec->Get_dblAngDef();
      str_trafo.sec.fases       = fases->ExisteFase(xmlEnrol_sec->Get_strFases());
      xmlZat_sec                = xmlEnrol_sec->Get_zTerra();
      str_trafo.sec.zterra.r    = xmlZat_sec->Get_dblR();
      str_trafo.sec.zterra.x    = xmlZat_sec->Get_dblX();

      xmlZSeq                   = xmlTrafo->Get_zSeq();
      xmlZ0                     = xmlZSeq->Get_z0();
      str_trafo.z0.r            = xmlZ0->Get_dblR();
      str_trafo.z0.x            = xmlZ0->Get_dblX();
      xmlZ1                     = xmlZSeq->Get_z1();
      str_trafo.z1.r            = xmlZ1->Get_dblR();
      str_trafo.z1.x            = xmlZ1->Get_dblX();
      str_trafo.perda_ferro     = xmlTrafo->Get_dblPerFer();

      // LTC - Load Tap Changer
      if ((xmlLtc = xmlTrafo->Get_ltc()) != NULL)
         {
			str_trafo.ltc.tipo     = xmlLtc->Get_intTpoUtl();
         //verifica se usa LTC
         if ((str_trafo.ltc.tipo == 1)||(str_trafo.ltc.tipo == 2))
            {//lê dados de LTC
            str_trafo.ltc.v_pu        = xmlLtc->Get_dblTns();

            if ((xmlCoordLTC_Id = xmlLtc->Get_coordBarCtl()) == NULL) return(false);
            if (xmlCoordLTC_Id->GetHasChildNodes() == true)
               {
               xmlCoordLTC_UTM      = xmlCoordLTC_Id->Get_coordUTM();
               xmlCoordLTC_ESQ1     = xmlCoordLTC_Id->Get_coordESQ1();
               xmlCoordLTC_ESQ2     = xmlCoordLTC_Id->Get_coordESQ2();
               /*
               str_trafo.ltc.bar_id = CriaBarra(xmlCoordLTC_Id->Get_intCoordNdx(), xmlCoordLTC_Id->Get_strCod(),
                                                xmlCoordLTC_UTM->Get_dblX(),       xmlCoordLTC_UTM->Get_dblY(),
                                                xmlCoordLTC_ESQ1->Get_dblX(),      xmlCoordLTC_ESQ1->Get_dblY(),
                                                xmlCoordLTC_ESQ2->Get_dblX(),      xmlCoordLTC_ESQ2->Get_dblY());
               */
               str_trafo.ltc.bar_id = CriaBarra(xmlCoordLTC_Id);
               }
            }
         }
      str_trafo.esq.posbar3     = 0;
      str_trafo.esq.comprimento = 0;

      // Insere Trafo em Monta
      if ((eqpto = monta->InsereTrafo(str_trafo)) == NULL) continue;

      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;

      //destrói objetos XML
      //delete xmlTrafo;         
      //delete xmlEqpto;         
      //delete xmlId;            
      //delete lstCoordId;       
      //delete xmlCoord1_Id;     
      //delete xmlCoord1_UTM;    
      //delete xmlCoord1_ESQ1;   
      //delete xmlCoord1_ESQ2;   
      //delete xmlCoord1_Ico;    
      //delete xmlCoord2_Id;     
      //delete xmlCoord2_UTM;    
      //delete xmlCoord2_ESQ1;   
      //delete xmlCoord2_ESQ2;   
      //delete xmlCoord2_Ico;    
      //delete xmlCoordLTC_Id;   
      //delete xmlCoordLTC_UTM;  
      //delete xmlCoordLTC_ESQ1; 
      //delete xmlCoordLTC_ESQ2; 
      //delete xmlZSeq;          
      //delete xmlZ0;            
      //delete xmlZ1;            
      //delete xmlZat_pri;       
      //delete xmlZat_sec;       
      //delete xmlLtc;           
      //delete lstEnrol;         
      //delete xmlEnrol_pri;     
      //delete xmlEnrol_sec;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoTrafo3E(_di_IXMLTEqLigTrafo3EList lstTrafo3E, VTRede* rede)
   {
   VTEqpto*                      eqpto;
   strTRAFO3E                    str_trafo3E;          // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigTrafo3E         xmlTrafo3E;           // Trafo de 3 enrolamentos
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoordId;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoordUTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoordESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoordESQ2;         // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoordIco;          // Deslocamentos para posicionamento dos ícones
   _di_IXMLTZseq                 xmlZSeq;              // Impedância de seqüência
   _di_IXMLTZ                    xmlZ;                 // Impedância
   _di_IXMLTEnrolList             lstEnrol;             // Enrolamentos
   _di_IXMLTEnrol                xmlEnrol;             // Enrolamento

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstTrafo3E) return(true);

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede) return(false);

   for (int i = 0; i < lstTrafo3E->Count; i++)
      {
      if ((xmlTrafo3E = lstTrafo3E->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto   = xmlTrafo3E->Get_eqlig()) == NULL) return(false);
      if ((xmlId      = xmlEqpto->Get_id())      == NULL) return(false);

      str_trafo3E.trf_id  = xmlId->Get_intId();
      str_trafo3E.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 3) return(false);

      xmlCoordId                = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM               = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1              = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2              = xmlCoordId->Get_coordESQ2();
      str_trafo3E.bar1_id       = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_trafo3E.bar1_id       = CriaBarra(xmlCoordId);
      str_trafo3E.esq.posbar1   = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordId                = lstCoordId->Get_Item(1);
      /*
      xmlCoordUTM               = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1              = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2              = xmlCoordId->Get_coordESQ2();
      str_trafo3E.bar2_id       = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_trafo3E.bar2_id       = CriaBarra(xmlCoordId);
      str_trafo3E.esq.posbar2   = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordId                = lstCoordId->Get_Item(2);
      /*
      xmlCoordUTM               = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1              = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2              = xmlCoordId->Get_coordESQ2();
      str_trafo3E.bar3_id       = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_trafo3E.bar3_id       = CriaBarra(xmlCoordId);
      str_trafo3E.esq.posbar3   = xmlCoordId->Get_dblPosBar();  // unck@20080807

      str_trafo3E.codigo        = xmlId->Get_strCod();
      str_trafo3E.extern_id     = xmlId->Get_strExtId();                        // Código externo. Inicialmente o mesmo código do trafo

      lstEnrol = xmlTrafo3E->Get_Enrol();
      // Verifica o número de enrolamentos esperado.
      if (lstEnrol->Count != 3) return(false);
      xmlEnrol = lstEnrol->Get_Item(0);
      str_trafo3E.pri.ligacao   = xmlEnrol->Get_intTpoLig();
      str_trafo3E.pri.snom      = xmlEnrol->Get_dblPotNom();
      str_trafo3E.pri.vnom      = xmlEnrol->Get_dblTnsNom();
      str_trafo3E.pri.tap       = xmlEnrol->Get_dblTnsTap();
      str_trafo3E.pri.defasagem = xmlEnrol->Get_dblAngDef();
      str_trafo3E.pri.fases     = fases->ExisteFase(xmlEnrol->Get_strFases());
      xmlZ                      = xmlEnrol->Get_zTerra();
      str_trafo3E.pri.zterra.r  = xmlZ->Get_dblR();
      str_trafo3E.pri.zterra.x  = xmlZ->Get_dblX();
      xmlEnrol = lstEnrol->Get_Item(1);
      str_trafo3E.sec.ligacao   = xmlEnrol->Get_intTpoLig();
      str_trafo3E.sec.snom      = xmlEnrol->Get_dblPotNom();
      str_trafo3E.sec.vnom      = xmlEnrol->Get_dblTnsNom();
      str_trafo3E.sec.tap       = xmlEnrol->Get_dblTnsTap();
      str_trafo3E.sec.defasagem = xmlEnrol->Get_dblAngDef();
      str_trafo3E.sec.fases     = fases->ExisteFase(xmlEnrol->Get_strFases());
      xmlZ                      = xmlEnrol->Get_zTerra();
      str_trafo3E.sec.zterra.r  = xmlZ->Get_dblR();
      str_trafo3E.sec.zterra.x  = xmlZ->Get_dblX();
      xmlEnrol = lstEnrol->Get_Item(2);
      str_trafo3E.ter.ligacao   = xmlEnrol->Get_intTpoLig();
      str_trafo3E.ter.snom      = xmlEnrol->Get_dblPotNom();
      str_trafo3E.ter.vnom      = xmlEnrol->Get_dblTnsNom();
      str_trafo3E.ter.tap       = xmlEnrol->Get_dblTnsTap();
      str_trafo3E.ter.defasagem = xmlEnrol->Get_dblAngDef();
      str_trafo3E.ter.fases     = fases->ExisteFase(xmlEnrol->Get_strFases());
      xmlZ                      = xmlEnrol->Get_zTerra();
      str_trafo3E.ter.zterra.r  = xmlZ->Get_dblR();
      str_trafo3E.ter.zterra.x  = xmlZ->Get_dblX();

      str_trafo3E.perda_ferro   = xmlTrafo3E->Get_dblPerFer();

      xmlZSeq                   = xmlTrafo3E->Get_zSeqPS();
      xmlZ                      = xmlZSeq->Get_z0();
      str_trafo3E.zps0.r        = xmlZ->Get_dblR();
      str_trafo3E.zps0.x        = xmlZ->Get_dblX();
      xmlZ                      = xmlZSeq->Get_z1();
      str_trafo3E.zps1.r        = xmlZ->Get_dblR();
      str_trafo3E.zps1.x        = xmlZ->Get_dblX();
      xmlZSeq                   = xmlTrafo3E->Get_zSeqPT();
      xmlZ                      = xmlZSeq->Get_z0();
      str_trafo3E.zpt0.r        = xmlZ->Get_dblR();
      str_trafo3E.zpt0.x        = xmlZ->Get_dblX();
      xmlZ                      = xmlZSeq->Get_z1();
      str_trafo3E.zpt1.r        = xmlZ->Get_dblR();
      str_trafo3E.zpt1.x        = xmlZ->Get_dblX();
      xmlZSeq                   = xmlTrafo3E->Get_zSeqST();
      xmlZ                      = xmlZSeq->Get_z0();
      str_trafo3E.zst0.r        = xmlZ->Get_dblR();
      str_trafo3E.zst0.x        = xmlZ->Get_dblX();
      xmlZ                      = xmlZSeq->Get_z1();
      str_trafo3E.zst1.r        = xmlZ->Get_dblR();
      str_trafo3E.zst1.x        = xmlZ->Get_dblX();

      str_trafo3E.esq.comprimento = 0;


      // Insere Trafo3E em Monta
      if ((eqpto = monta->InsereTrafo3E(str_trafo3E)) == NULL) continue;
      // Insere o conjunto de medições
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;

      //destrói objetos XML
      //delete 
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoTrafoZZ(_di_IXMLTEqBarTrafoZZList lstTrafoZZ, VTRede* rede)
   {
   VTEqpto*                    eqpto;
   strTRAFOZZ                  str_TrafoZZ;     // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqBarTrafoZZ       xmlTrafoZZ;      // TrafoZZ
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTId                 xmlId;          // Id do equipamento
   _di_IXMLTCoordIdList         lstCoordId;     // Lista de coordenadas identificadas
   _di_IXMLTCoordId            xmlCoordId;     // Coordenada identificada (Barras)
   //_di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   //_di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   //_di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos ícones
   _di_IXMLTZ                  xmlZ;           // Impedância

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstTrafoZZ)
     return(true);

   for (int i = 0; i < lstTrafoZZ->Count; i++)
       {
         if ((xmlTrafoZZ = lstTrafoZZ->Get_Item(i)) == NULL) return(false);
         if ((xmlEqpto   = xmlTrafoZZ->Get_eqbar()) == NULL) return(false);
         if ((xmlId      = xmlEqpto->Get_id())      == NULL) return(false);

         str_TrafoZZ.id         = xmlId->Get_intId();
         str_TrafoZZ.codigo     = xmlId->Get_strCod();
         str_TrafoZZ.extern_id  = xmlId->Get_strExtId();                  // Código externo. Inicialmente o mesmo código do capacitor

         lstCoordId = xmlEqpto->Get_coordId();
         // Verifica o número de coordenadas esperado.
         if (lstCoordId->Count < 1) return(false);

         xmlCoordId             = lstCoordId->Get_Item(0);
         /*
         xmlCoordUTM            = xmlCoordId->Get_coordUTM();
         xmlCoordESQ1           = xmlCoordId->Get_coordESQ1();
         xmlCoordESQ2           = xmlCoordId->Get_coordESQ2();
         str_TrafoZZ.bar_id     = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                            xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                            xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                            xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
         */
         str_TrafoZZ.bar_id     = CriaBarra(xmlCoordId);
         str_TrafoZZ.esq.posbar  = xmlCoordId->Get_dblPosBar();  // unck@20080807

         xmlCoordIco            = xmlCoordId->Get_coordIco();
         str_TrafoZZ.esq.dxbar  = xmlCoordIco->Get_dblDxBar();   // unck@20080807
         str_TrafoZZ.esq.dybar  = xmlCoordIco->Get_dblDyBar();   // unck@20080807

         str_TrafoZZ.vnom       = xmlTrafoZZ->Get_dblTnsNom();
         str_TrafoZZ.snom       = xmlTrafoZZ->Get_dblPotNom();

         xmlZ                   = xmlTrafoZZ->Get_z0();
         str_TrafoZZ.z0.r       = xmlZ->Get_dblR();
         str_TrafoZZ.z0.x       = xmlZ->Get_dblX();

         // Insere TrafoZZ em Monta
       //if ((eqpto = monta->InsereTrafoZZ(str_TrafoZZ)) == NULL) return(false);
       if ((eqpto = monta->InsereTrafoZZ(str_TrafoZZ)) == NULL) continue;
       // Insere o conjunto de medições
       //if (!InsereMedicao(xmlEqpto, eqpto)) return(false);
       if (! InsereMedicao(xmlEqpto, eqpto)) continue;
       }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereConjuntoTrecho(_di_IXMLTEqLigTrechoList lstTrecho, VTRede* rede)
   {
   //double                        utm_x1, utm_y1, utm_x2, utm_y2;
   VTEqpto*                      eqpto;
   strTRECHO                     str_trecho;           // Estrutura de armazenamento a ser inserida em TMonta
   strARRANJO1                   str_arranjo;          // Estrutura de armazenamento a ser inserida em TMonta
   _di_IXMLTEqLigTrecho          xmlTrecho;            // Trafo de 3 enrolamentos
   _di_IXMLTEqpto                xmlEqpto;             // Eqpto
   _di_IXMLTId                   xmlId;                // Id do equipamento
   _di_IXMLTCoordIdList           lstCoordId;           // Lista de coordenadas identificadas
   _di_IXMLTCoordId              xmlCoordId;           // Coordenada identificada (Barras)
   //_di_IXMLTCoord                xmlCoordUTM;          // Coordenada UTM
   //_di_IXMLTCoord                xmlCoordESQ1;         // Coordenada Esquema 1
   //_di_IXMLTCoord                xmlCoordESQ2;         // Coordenada Esquema 2
   _di_IXMLTCoordIco             xmlCoordIco;          // Deslocamentos para posicionamento dos ícones
   _di_IXMLTZseq                 xmlZSeq;              // Impedância de seqüência
   _di_IXMLTZ                    xmlZ;                 // Impedância
   _di_IXMLTCSeq                 xmlCSeq;              // Capacitância de seqüência
   _di_IXMLTTrcTpo               xmlTrcTpo;            // Tipo de trecho
   _di_IXMLTTrcTpo01             xmlTrcTpo01;          // Trecho tipo 01
   _di_IXMLTTrcTpo02             xmlTrcTpo02;          // Trecho tipo 02
   _di_IXMLTTrcTpo03             xmlTrcTpo03;          // Trecho tipo 03
   _di_IXMLTCaboId               xmlCabo;              // Identificador de um cabo

   // Testa o ponteiro passado. Se for nulo, retorna true, pois não consiste erro
   if (!lstTrecho)
     return(true);

   // Testa o ponteiro passado. Se for nulo, retorna false, pois precisa associar a uma rede
   if (!rede)
     return(false);

   for (int i = 0; i < lstTrecho->Count; i++)
      {
      if ((xmlTrecho = lstTrecho->Get_Item(i)) == NULL) return(false);
      if ((xmlEqpto  = xmlTrecho->Get_eqlig()) == NULL) return(false);
      if ((xmlId     = xmlEqpto->Get_id())     == NULL) return(false);

      str_trecho.tre_id  = xmlId->Get_intId();
      str_trecho.rede_id = rede->Id;

      lstCoordId = xmlEqpto->Get_coordId();
      // Verifica o número de coordenadas esperado.
      if (lstCoordId->Count < 2) return(false);

      xmlCoordId                 = lstCoordId->Get_Item(0);
      /*
      xmlCoordUTM                = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1               = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2               = xmlCoordId->Get_coordESQ2();
      utm_x1                     = xmlCoordUTM->Get_dblX();
      utm_y1                     = xmlCoordUTM->Get_dblY();
      str_trecho.bar1_id         = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                             xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                             xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                             xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_trecho.bar1_id         = CriaBarra(xmlCoordId);
      str_trecho.esq.posbar1     = xmlCoordId->Get_dblPosBar();  // unck@20080807

      xmlCoordId                 = lstCoordId->Get_Item(1);
      /*
      xmlCoordUTM                = xmlCoordId->Get_coordUTM();
      xmlCoordESQ1               = xmlCoordId->Get_coordESQ1();
      xmlCoordESQ2               = xmlCoordId->Get_coordESQ2();
      utm_x2                     = xmlCoordUTM->Get_dblX();
      utm_y2                     = xmlCoordUTM->Get_dblY();
      str_trecho.bar2_id         = CriaBarra(xmlCoordId->Get_intCoordNdx(), xmlCoordId->Get_strCod(),
                                             xmlCoordUTM->Get_dblX(),       xmlCoordUTM->Get_dblY(),
                                             xmlCoordESQ1->Get_dblX(),      xmlCoordESQ1->Get_dblY(),
                                             xmlCoordESQ2->Get_dblX(),      xmlCoordESQ2->Get_dblY());
      */
      str_trecho.bar2_id         = CriaBarra(xmlCoordId);
      str_trecho.esq.posbar2     = xmlCoordId->Get_dblPosBar();  // unck@20080807

      str_trecho.codigo          = xmlId->Get_strCod();
      str_trecho.extern_id       = xmlId->Get_strExtId();                       // Código externo. Inicialmente o mesmo código do trecho
      str_trecho.comp_km         = xmlTrecho->Get_dblCmp();

      // DEPOOPS: Fui obrigado a forçar um valor fixo em vnom da barra. Caso contrário, janelada ao incluir o trecho
      // Divisão por zero em CriaArranjoCEPEL chamando DefC0C1_pu

      if ((xmlTrcTpo   = xmlTrecho->Get_trcTpo())   == NULL) return(false);

      //obtém código do Arranjo
      str_arranjo.codigo = xmlTrcTpo->Get_strCodArr();

      if ((xmlTrcTpo01 = xmlTrcTpo->Get_trcTpo01()) == NULL) return(false);
      if ((xmlTrcTpo02 = xmlTrcTpo->Get_trcTpo02()) == NULL) return(false);
      if ((xmlTrcTpo03 = xmlTrcTpo->Get_trcTpo03()) == NULL) return(false);

      if (xmlTrcTpo01->HasChildNodes)
          {// Trecho tipo 1 (ini): Trecho tipo 2 (ini)
          if ((xmlCabo = xmlTrcTpo01->Get_caboId()) == NULL) return(false);
          str_arranjo.tipo_cfg              = arranjoCABO_Z0Z1;
          str_arranjo.cfg_caboZ0Z1.cod_cabo = xmlCabo->GetText();
          }
      else if (xmlTrcTpo02->HasChildNodes)
          {// Trecho tipo 2 (ini): arranjoCABOxFASE
          str_arranjo.tipo_cfg                   = arranjoCABOxFASE;
          str_arranjo.cfg_suporte_cabo.estgeo_id = xmlTrcTpo02->Get_strEstGeoId();

//JCG início
//           // Caso o suporte não exista, deve ser inserido.
//           if ((arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos))) == NULL) return(false);
//           if (!arranjos->ExisteSuporte(estgeo_id))
//               {
//               //suporte = NewObjSuporte();                                    // OOPS: Não linka.
//               //suporte->Codigo = estgeo_id;                                  // OOPS: e os demais atributos???
//               //arranjos->InsereSuporte(suporte);
//               }
//JCG fim

         if ((xmlCabo = xmlTrcTpo02->Get_caboIdA()) == NULL) return(false);
         str_arranjo.cfg_suporte_cabo.cod_caboA = xmlCabo->GetText();
         if ((xmlCabo = xmlTrcTpo02->Get_caboIdB()) == NULL) return(false);
         str_arranjo.cfg_suporte_cabo.cod_caboB = xmlCabo->GetText();
         if ((xmlCabo = xmlTrcTpo02->Get_caboIdC()) == NULL) return(false);
         str_arranjo.cfg_suporte_cabo.cod_caboC = xmlCabo->GetText();
         if ((xmlCabo = xmlTrcTpo02->Get_caboIdN()) == NULL) return(false);
         str_arranjo.cfg_suporte_cabo.cod_caboN = xmlCabo->GetText();
         // Trecho tipo 2 (fim)
         }
      else if (xmlTrcTpo03->HasChildNodes)
          {// Trecho tipo 3 (ini): arranjoZ0Z1
          str_arranjo.tipo_cfg           = arranjoZ0Z1;

          xmlZSeq                         = xmlTrcTpo03->Get_zSeq();
          xmlZ                            = xmlZSeq->Get_z0();
          str_arranjo.cfg_arranjoZ0Z1.z0.r = xmlZ->Get_dblR();
          str_arranjo.cfg_arranjoZ0Z1.z0.x = xmlZ->Get_dblX();
          xmlZ                            = xmlZSeq->Get_z1();
          str_arranjo.cfg_arranjoZ0Z1.z1.r = xmlZ->Get_dblR();
          str_arranjo.cfg_arranjoZ0Z1.z1.x = xmlZ->Get_dblX();

          xmlCSeq                          = xmlTrcTpo03->Get_cSeq();
          str_arranjo.cfg_arranjoZ0Z1.c0   = xmlCSeq->Get_dblC0();
          str_arranjo.cfg_arranjoZ0Z1.c1   = xmlCSeq->Get_dblC1();

          str_arranjo.cfg_arranjoZ0Z1.iadm_amp = xmlTrcTpo03->Get_dblCorMax();
          // Trecho tipo 3 (fim)
          }
      else
         {//erro
         return(false);
         }

      str_trecho.esq.posbar3     = 0;
      str_trecho.esq.comprimento = 0;
      //verifica compatibilidade entre comprimento e coordenadas
      //str_trecho.comp_km = ValidaComprimento(utm_x1, utm_y1, utm_x2, utm_y2, str_trecho.comp_km);

      // Insere o trecho série em monta
      //if ((eqpto = monta->InsereTrecho(str_trecho)) == NULL) return(false);
      if ((eqpto = monta->InsereTrecho(str_trecho, str_arranjo)) == NULL) continue;
      // Insere o conjunto de medições
      //if (!InsereMedicao(xmlEqpto, eqpto)) return(false);
      if (! InsereMedicao(xmlEqpto, eqpto)) continue;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaXML::InsereMedicao(_di_IXMLTEqpto xmlEqpto, VTEqpto* eqpto)
   {
   strCANAL              str_canal;
   _di_IXMLTMedicaoList  cnjMedicao;     // Lista de Medições
   _di_IXMLTMedicao      xmlMedicao;     // Medicao (Medidor e Canal)
   _di_IXMLTId           xmlId;          // Deslocamentos para posicionamento dos ícones
   _di_IXMLTPatamPropList cnjPatamar;
   _di_IXMLTPatamProp    xmlPatamar;
   _di_IXMLTHorMin       xmlHorMin;

   cnjMedicao = xmlEqpto->Get_Medicao();
   //proteção: a maior parte dos Eqptos não tem medição
   if (cnjMedicao->Count == 0) return(true);
   //loop p/ todas medições do eqpto
   for (int i = 0; i < cnjMedicao->Count; i++)
      {
      try{
         if ((xmlMedicao = cnjMedicao->Get_Item(i)) == NULL) return(false);
         xmlId = xmlMedicao->Get_medidorId();
         str_canal.canal_id          = -1;
         str_canal.medidor_id        = xmlId->Get_intId();
         xmlId                       = xmlMedicao->Get_canalId();
         str_canal.codigo            = xmlId->Get_strCod();
         str_canal.tipo_canal        = xmlMedicao->Get_intTpoCnl();
         str_canal.tolerancia_perc   = xmlMedicao->Get_dblSigmaCnl();
         str_canal.eqpto_medido.tipo = eqpto->Tipo();
         str_canal.eqpto_medido.id   = eqpto->Id;

         cnjPatamar = xmlMedicao->Get_Patamar();
         //monta curva
         str_canal.curva.id       = -1;
         str_canal.curva.tipica   = false;
         str_canal.curva.num_pat  = cnjPatamar->Count;
         str_canal.curva.num_valor = nvCURVA_PQVT;
         for (int j = 0; j < cnjPatamar->Count; j++)
            {
            if ((xmlPatamar = cnjPatamar->Get_Item(j)) == NULL) return(false);
            xmlHorMin                 = xmlPatamar->Get_horIni();
            str_canal.curva.patamar[j].hm_ini.hora   = xmlHorMin->Get_intHor();
            str_canal.curva.patamar[j].hm_ini.minuto = xmlHorMin->Get_intMin();
            xmlHorMin                 = xmlPatamar->Get_horFim();
            str_canal.curva.patamar[j].hm_fim.hora   = xmlHorMin->Get_intHor();
            str_canal.curva.patamar[j].hm_fim.minuto = xmlHorMin->Get_intMin();
            str_canal.curva.patamar[j].valor[indP]     = xmlPatamar->Get_dblPotAtv() / 1000.; //kW   => MW
            str_canal.curva.patamar[j].valor[indQ]     = xmlPatamar->Get_dblPotRtv() / 1000.; //kVAr => MVAr
            str_canal.curva.patamar[j].valor[indV]     = xmlPatamar->Get_dblMdlTns();
            str_canal.curva.patamar[j].valor[indT]     = xmlPatamar->Get_dblAng();
            }
         if    (!monta->InsereMedidorCanal(str_canal)) return(false);
         }catch(Exception &e)
            {//valida o formato do objeto Medicao
            formato->ValidaObjetoMedicao(xmlMedicao);
            continue;
            }
      }
   return(true);
   } // InsereMedicao

//---------------------------------------------------------------------------
VTRede* __fastcall TImportaXML::InsereRede(_di_IXMLTRede xmlRede)
   {
   //variáveis locais
   AnsiString   codigo;
   strREDE      str_rede;
   _di_IXMLTId  xmlId;
   VTEqpto      *cluster;
   VTEqpto      *dominio;
   VTEqpto      *eqpto;

   //obtém XMLTId
   xmlId = xmlRede->Get_id();
   //insere Cluster e  Dominio, se houver
   cluster = InsereCluster(xmlRede->Get_strCluster());
   dominio = InsereDominio(xmlRede->Get_strDominio());
   //preenche estrutura strREDE
   str_rede.rede_id            = xmlId->Get_intId();
   str_rede.codigo             = xmlId->Get_strCod();
   str_rede.mrede_id           = -1;
   str_rede.cluster_id         = (cluster != NULL) ? cluster->Id : 0;
   str_rede.dominio_id         = (dominio != NULL) ? dominio->Id : 0;
   str_rede.barini_id          = -1;
   str_rede.tiporede.id        = -1;
   str_rede.tiporede.segmento  = -1;
   str_rede.tiporede.codigo    = xmlRede->Get_strTpo();
   str_rede.estatica           = true;
   str_rede.radial             = false;
   str_rede.equivalente        = false;
   str_rede.color              = clBlack;
   //insere Rede em Monta
   if ((eqpto = monta->InsereRede(str_rede)) == NULL) return(NULL);
   return((VTRede*)eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TImportaXML::MsgLog(AnsiString txt, VTRede* rede)
   {
   if (rede != NULL)
      {
      txt = "Rede " + AnsiQuotedStr(rede->Codigo, '\'') + ": " + txt;
      }
   plog->Add(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TImportaXML::RelatorioLog(TStrings *lines)
   {
   lines->Clear();
   lines->LoadFromFile(plog->ArqLog());
   }

//---------------------------------------------------------------------------
double __fastcall TImportaXML::ValidaComprimento(double x1_m, double y1_m,
                                                 double x2_m, double y2_m, double comp_km)
   {
   //variáveis locais
   double dx_m, dy_m, dist_coord;

   //verifica se foi fornecido o comprimento
   if (comp_km > 0) return(comp_km);
   //proteção: verifica se foram definidas as coordenadas utm
   if ((x1_m <= 0.)||(y1_m <= 0.)) return(comp_km);
   if ((x2_m <= 0.)||(y2_m <= 0.)) return(comp_km);
   //calcula comprimento definido pelas coordenadas
   dx_m = fabs(x2_m - x1_m);
   dy_m = fabs(y2_m - y1_m);
   dist_coord = (sqrt((dx_m * dx_m) + (dy_m * dy_m))) / 1000.;
   //valida comprimento
   if ((comp_km <= 0)||(comp_km > 1.0e5)||(! IsDoubleZero(comp_km - dist_coord, 0.001)))
      {//redefine comp_km
      comp_km = dist_coord;
      }
   return(comp_km);
   }

//-----------------------------------------------------------------------------
// eof



