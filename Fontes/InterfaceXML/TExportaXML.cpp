//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaXML.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapSerie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
class VTSuporte;

//---------------------------------------------------------------------------
VTExportaXML* NewObjExportaXML(VTApl *apl_owner)
   {
   return(new TExportaXML(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TExportaXML::TExportaXML(VTApl *apl_owner)
   {
   //cria objeto Apl próprio
   apl = NewObjApl(NULL, apl_owner);
   apl->Add(DLL_NewObjFases());
   //cria lista
   lisEQP  = new TList();
   //assume opções default
   ExportaTipoChave   = true;
   ExportaTipoRede    = true;
   ExportaCurvaTipica = true;
   ExportaArranjo     = true;
   ExportaCabo        = true;
   ExportaSuporte     = true;
   ExportaRede        = true;
   ExportaReducao     = false;
   // Configura a codificação do arquivo
   strEncoding     = "WINDOWS-1252";
   }

//---------------------------------------------------------------------------
__fastcall TExportaXML::~TExportaXML(void)
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

//---------------------------------------------------------------------------
void __fastcall TExportaXML::AjustaCoordenadaBarra(void)
   {
   /*
   //variáveis locais
   int     index;
   VTBarra *barra1, *barra2;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisBAR = redes->LisBarra();

   //inicia toda Barra c/ index igual a 0
   for (int i = 0; i < lisBAR->Count; i++)
      {
      barra1 = (VTBarra*)lisBAR->Items[i];
      barra1->index = 0;
      }
   //procura pelas Barras de mesma coordenada utm
   for (int i = 0; i < lisBAR->Count; i++)
      {
      barra1 = (VTBarra*)lisBAR->Items[i];
      //verifica se o index da Barra já foi iniciado
      if (barra1->index > 0) continue;
      //verifica se a Barra possui coord.utm
      if ((barra1->utm.x == 0)||(barra1->utm.y == 0)) continue;
      //inicia index p/ as Barras de mesma coord.utm
      index = 1;
      //verifica coord.utm das demais Barras
      for (int j = i+1; j < lisBAR->Count; j++)
         {
         barra2 = (VTBarra*)lisBAR->Items[j];
         if (barra2->index > 0) continue;
         if ((barra2->utm.x == barra1->utm.x)&&
             (barra2->utm.y == barra1->utm.y))
            {//redefine index da Barra de referência
            barra1->index  = 1;
            //inicia index da Barra de mesma coordenada
            barra2->index  = ++index;
            }
         }
      }
   */
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::ExportaRedes(AnsiString arq_xml)
   {
   //variáveis locais
   byte separador;
   bool sucesso;

   try{//proteção: destrói eventual instância anterior
      //   unck@20080417
      //   Se não deletar o CodeGuard não reclama.
      //   if (xmlConcessAux) {delete xmlConcessAux; xmlConcessAux = NULL;}
      //cria objetos XML
      xmlConcessAux = NewConcessionaria();
      //salva e redefine separador decimal
      separador = FormatSettings.DecimalSeparator;
      FormatSettings.DecimalSeparator = '.';
      //monta dados da rede completa
      sucesso = Monta(NULL);
      //salva dados no arquivo indicado
      xmlConcessAux->OwnerDocument->SetEncoding(strEncoding);
      if (sucesso) xmlConcessAux->OwnerDocument->SaveToFile(arq_xml);
      //restaura separador decimal
      FormatSettings.DecimalSeparator = separador;
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::ExportaUmaRede(VTRede *rede, AnsiString arq_xml)
   {
   //variáveis locais
   byte separador;
   bool sucesso;

   try{//cria objetos XML
      xmlConcessAux = NewConcessionaria();
      //salva e redefine separador decimal
      separador = FormatSettings.DecimalSeparator;
      FormatSettings.DecimalSeparator = '.';
      //monta dados da rede
      sucesso = Monta(rede);
      //salva dados no arquivo indicado
      xmlConcessAux->OwnerDocument->SetEncoding(strEncoding);
      if (sucesso) xmlConcessAux->OwnerDocument->SaveToFile(arq_xml);
      //restaura separador decimal
      FormatSettings.DecimalSeparator = separador;
      }catch(Exception &e)
         {
         return(false);
         }
   return(sucesso);
  }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::ExportaArranjos(AnsiString arq_xml)
   {
   //variáveis locais
   byte       separador;
   bool       sucesso;

   try{//cria objetos XML
      xmlConcessAux = NewConcessionaria();
      //salva e redefine separador decimal
      separador = FormatSettings.DecimalSeparator;
      FormatSettings.DecimalSeparator = '.';
      //monta header
      sucesso = MontaConjuntoConcessionaria();
      //monta conjunto de cabos
      if (sucesso) sucesso = MontaConjuntoCaboMonopolar();
      if (sucesso) sucesso = MontaConjuntoCaboMultiplexado();
      //monta conjunto de suportes
      if (sucesso) sucesso = MontaConjuntoSuporte();
      //monta conjunto de suportes
      if (sucesso) sucesso = MontaConjuntoArranjo();
      //salva dados no arquivo indicado
      xmlConcessAux->OwnerDocument->SetEncoding(strEncoding);
      if (sucesso) xmlConcessAux->OwnerDocument->SaveToFile(arq_xml);
      //restaura separador decimal
      FormatSettings.DecimalSeparator = separador;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
  }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::ExportaCurvasTipicas(AnsiString arq_xml)
   {
   //variáveis locais
   byte       separador;
   bool       sucesso;

   try{//cria objetos XML
      xmlConcessAux = NewConcessionaria();
      //salva e redefine separador decimal
      separador = FormatSettings.DecimalSeparator;
      FormatSettings.DecimalSeparator = '.';
      //monta header
      sucesso = MontaConjuntoConcessionaria();
      //monta conjunto das curvas
      if (sucesso) sucesso = MontaConjuntoCurvasTipicas();
      //salva dados no arquivo indicado
      xmlConcessAux->OwnerDocument->SetEncoding(strEncoding);
      if (sucesso) xmlConcessAux->OwnerDocument->SaveToFile(arq_xml);
      //restaura separador decimal
      FormatSettings.DecimalSeparator = separador;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
  }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::Monta(VTRede *rede)
   {
   //ajusta coordenadas das barras sobrepostas
   if (ExportaRede) AjustaCoordenadaBarra();
   //monta conjunto de controle
   if (! MontaConjuntoConcessionaria())     return(false);
   //monta conjunto de tipos de redes
   if (ExportaTipoRede)
      {
      if (! MontaConjuntoTipoRede())         return(false);
      }
   //monta conjunto de tipos de chaves
   if (ExportaTipoChave)
      {
      if (! MontaConjuntoTipoChave())        return(false);
      }
   //monta conjunto de suportes, cabos e arranjos
   if (ExportaArranjo)
      {
      if (! MontaConjuntoSuporte())          return(false);
      if (! MontaConjuntoCaboMonopolar())    return(false);
      if (! MontaConjuntoCaboMultiplexado()) return(false);
      if (! MontaConjuntoArranjo())          return(false);
      }
   //monta conjunto de curvas típicas
   if (ExportaCurvaTipica)
      {
      if (! MontaConjuntoCurvasTipicas()) return(false);
      }
   //monta conjunto de redes ou apenas uma rede
   if (ExportaRede)
      {
      if (rede != NULL)
         {
         if (! MontaUnicaRede(rede))     return(false);
         }
      else
         {
         if (! MontaConjuntoAllRede())     return(false);
         }
      }
   //monta conjunto de redes equivalentes
   if (ExportaRede && ExportaReducao)
      {
      if (! MontaConjuntoReducao())       return(false);
      }
   return(true);
  }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoAllRede(void)
   {
   //variáveis locais
   VTRedes          *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList            *lisREDE = redes->LisRede();
   VTRede           *rede;

   try{//obtém objeto xmlLstRede
      //loop p/ todo objeto Rede
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         MontaUnicaRede(rede);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoArranjo(void)
   {
   TList*      lisArranjo = NULL;
   VTArranjo*  arranjo    = NULL;
   VTArranjos* arranjos   = (VTArranjos*)apl->GetObject(__classid(VTArranjos));

   if ((lisArranjo = arranjos->LisArranjo()) == NULL) return(false);
   for (int i = 0; i < lisArranjo->Count; i++)
      {
      if ((arranjo = (VTArranjo*)lisArranjo->Items[i]) == NULL) return(false);

      switch (arranjo->TipoCfg)
         {
         case arranjoZ0Z1      : if (!MontaConjuntoArranjoZ0Z1(arranjo))     return(false);  break;
         case arranjoCABO_Z0Z1 : if (!MontaConjuntoArranjoCaboZ0Z1(arranjo)) return(false);  break;
         case arranjoCABOxFASE : if (!MontaConjuntoArranjoCaboRGMR(arranjo)) return(false);  break;
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoArranjoCaboRGMR(VTArranjo* arranjo)
   {
   int                           fase_tag;
   _di_IXMLTArranjoCaboRGMRList   xmlArranjoList;
   _di_IXMLTArranjoCaboRGMR      xmlArranjo;
   _di_IXMLTId                   xmlId;
   _di_IXMLTSuportePtoFixCaboList xmlPtoFixList;
   _di_IXMLTSuportePtoFixCabo    xmlPtoFix;
   VTFases*                      fases;
   TList*                        lisPtoFixacao;
   int                           intCaboId;

   try
      {
      // obtém a lista de pontos de fixação
      lisPtoFixacao = arranjo->suporte->LisPtoFixacao();
      // obtém o objeto fases
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);
      // obtém a lista de arranjos
      if ((xmlArranjoList = xmlConcessAux->Get_ArranjoCaboRGMR()) == NULL) return(false);
      // Insere um novo arranjo na lista
      if ((xmlArranjo = xmlArranjoList->Add()) == NULL)                    return(false);

      // Obtém as referâncias para os demais campos
      if ((xmlId = xmlArranjo->Get_id()) == NULL)                         return(false);
      if ((xmlPtoFixList = xmlArranjo->Get_PtoFix()) == NULL)              return(false);

      // Configura os valores dos campos
      xmlId->Set_intId(arranjo->Id);
      xmlId->Set_strCod(arranjo->Codigo);
      xmlArranjo->Set_strFases(fases->AsStrABCN[arranjo->Fases]);
      xmlArranjo->Set_intSuporteId(arranjo->suporte->Id);
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {//determina tag da fase
         fase_tag = fases->IndexToTag[ind_fase];
         //verifica se existe a fase
         if ((arranjo->Fases & fase_tag) == fase_tag)
            {
            if ((xmlPtoFix = xmlPtoFixList->Add()) == NULL) return(false);
            //ID do PtoFixação (número sequencial a partir de 0 que equivale ao índice do PtoFixacao na lista)
            xmlPtoFix->Set_intId(lisPtoFixacao->IndexOf(arranjo->fase[ind_fase].pto_fixacao));
            // fase do PtoFixacao
            xmlPtoFix->Set_strFase(fases->AsStrABCN[fase_tag]);
            //Id do cabo
            arranjo->fase[ind_fase].cabo ? intCaboId = arranjo->fase[ind_fase].cabo->Id : intCaboId = -1;
            xmlPtoFix->Set_intCaboId(intCaboId);
            }
         }
      }
   catch (Exception& e)
      {
      return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoArranjoCaboZ0Z1(VTArranjo* arranjo)
   {
   _di_IXMLTArranjoCaboZ0Z1List xmlArranjoList;
   _di_IXMLTArranjoCaboZ0Z1    xmlArranjo;
   _di_IXMLTId                 xmlId;
   VTFases*                    fases;

   try
      {
      // obtém o objeto fases
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);
      // obtém a lista de arranjos
      if ((xmlArranjoList = xmlConcessAux->Get_ArranjoCaboZ0Z1()) == NULL) return(false);
      // Insere um novo arranjo na lista
      if ((xmlArranjo = xmlArranjoList->Add()) == NULL)                    return(false);

      // Obtém as referâncias para os demais campos
      if ((xmlId = xmlArranjo->Get_id()) == NULL)                         return(false);

      // Configura os valores dos campos
      xmlId->Set_intId(arranjo->Id);
      xmlId->Set_strCod(arranjo->Codigo);
      xmlArranjo->Set_strFases(fases->AsStrABCN[arranjo->Fases]);
      xmlArranjo->Set_intCaboId(arranjo->caboZ0Z1->Id);
      }
   catch (Exception& e)
      {
      return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoArranjoZ0Z1(VTArranjo* arranjo)
   {
   _di_IXMLTArranjoZ0Z1List xmlArranjoList;
   _di_IXMLTArranjoZ0Z1    xmlArranjo;
   _di_IXMLTId             xmlId;
   _di_IXMLTCSeq           xmlCSeq;
   _di_IXMLTZseq           xmlZSeq;
   _di_IXMLTZ              xmlZ0;
   _di_IXMLTZ              xmlZ1;
   VTFases*                fases;

   try
      {
      // obtém o objeto fases
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);
      // obtém a lista de arranjos
      if ((xmlArranjoList = xmlConcessAux->Get_ArranjoZ0Z1()) == NULL)     return(false);
      // Insere um novo arranjo na lista
      if ((xmlArranjo = xmlArranjoList->Add()) == NULL)                    return(false);

      // Obtém as referâncias para os demais campos
      if ((xmlId = xmlArranjo->Get_id()) == NULL)                         return(false);
      if ((xmlCSeq = xmlArranjo->Get_dblCSeq()) == NULL)                  return(false);
      if ((xmlZSeq = xmlArranjo->Get_zSeq()) == NULL)                     return(false);
      if ((xmlZ0 = xmlZSeq->Get_z0()) == NULL)                            return(false);
      if ((xmlZ1 = xmlZSeq->Get_z1()) == NULL)                            return(false);

      // Configura os valores dos campos
      xmlId->Set_intId(arranjo->Id);
      xmlId->Set_strCod(arranjo->Codigo);
      xmlArranjo->Set_strFases(fases->AsStrABCN[arranjo->Fases]);
      xmlArranjo->Set_dblCorMax(arranjo->Iadm_amp);
      xmlZ0->Set_dblR(arranjo->z0.r);
      xmlZ0->Set_dblX(arranjo->z0.x);
      xmlZ1->Set_dblR(arranjo->z1.r);
      xmlZ1->Set_dblX(arranjo->z1.x);
      xmlCSeq->Set_dblC0(arranjo->c0);
      xmlCSeq->Set_dblC1(arranjo->c1);
      }
   catch (Exception& e)
      {
      return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCaboMonopolar(void)
   {
   //variáveis locais
   _di_IXMLTCaboRGMRList xmlCaboList;
   _di_IXMLTCaboRGMR    xmlCabo;
   _di_IXMLTId          xmlId;
   VTCabo               *cabo;
   VTArranjos           *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList                *lisCABO  = arranjos->LisCabo();

   try{//obtém objeto xmlCaboList
      if ((xmlCaboList = xmlConcessAux->Get_CaboRGMR()) == NULL) return(false);
      //loop p/ todos Cabos
      for (int n = 0; n < lisCABO->Count; n++)
         {
         cabo = (VTCabo*)lisCABO->Items[n];
         //verifica se o cabo é do tipo caboR_GMR
         if (cabo->TipoCfg != caboR_GMR) continue;
         //descarta Cabo com código não definido
         if (cabo->Codigo.IsEmpty()) continue;
         //insere um novo Cabo
         if ((xmlCabo = xmlCaboList->Add()) == NULL) return(false);
         if ((xmlId   = xmlCabo->Get_id()) == NULL) return(false);
         //copia dados do cabo
         xmlId->Set_intId(cabo->Id);
         xmlId->Set_strCod(cabo->Codigo);
         xmlCabo->Set_dblCorMax(cabo->Iadm_A);
         xmlCabo->Set_dblR(cabo->r);
         xmlCabo->Set_dblGMR(cabo->gmr);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCaboMultiplexado(void)
   {
   //variáveis locais
   _di_IXMLTCaboZ0Z1List xmlCaboList;
   _di_IXMLTCaboZ0Z1    xmlCabo;
   _di_IXMLTId          xmlId;
   _di_IXMLTCSeq        xmlCSeq;
   _di_IXMLTZseq        xmlZSeq;
   _di_IXMLTZ           xmlZ;
   VTCabo               *cabo;
   VTArranjos           *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList                *lisCABO  = arranjos->LisCabo();

   try{//obtém objeto xmlCaboList
      if ((xmlCaboList = xmlConcessAux->Get_CaboZ0Z1()) == NULL) return(false);
      //loop p/ todos Cabos
      for (int n = 0; n < lisCABO->Count; n++)
         {
         cabo = (VTCabo*)lisCABO->Items[n];
         //verifica se o cabo é do tipo caboR_GMR
         if ((cabo->TipoCfg != caboZ0Z1)&&(cabo->TipoCfg != caboPRE_REUNIDO)) continue;
         //descarta Cabo com código não definido
         if (cabo->Codigo.IsEmpty()) continue;
         //insere novo objeto xmlCabo
         if ((xmlCabo = xmlCaboList->Add())      == NULL) return(false);
         if ((xmlId   = xmlCabo->Get_id())      == NULL) return(false);
         if ((xmlCSeq = xmlCabo->Get_dblCSeq()) == NULL) return(false);
         if ((xmlZSeq = xmlCabo->Get_zSeq())    == NULL) return(false);
         //copia dados do cabo
         xmlId->Set_intId(cabo->Id);
         xmlId->Set_strCod(cabo->Codigo);
         xmlCabo->Set_dblCorMax(cabo->Iadm_A);
         //impedâncias de seq.0
         xmlZ = xmlZSeq->Get_z0();
         xmlZ->Set_dblR(cabo->z0.r);
         xmlZ->Set_dblX(cabo->z0.x);
         //impedâncias de seq.1
         xmlZ = xmlZSeq->Get_z1();
         xmlZ->Set_dblR(cabo->z1.r);
         xmlZ->Set_dblX(cabo->z1.x);
         //capacitâncias
         xmlCSeq->Set_dblC0(cabo->c0);
         xmlCSeq->Set_dblC1(cabo->c1);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCapacitor(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTCapacitor*                capacitor;
   _di_IXMLTEqpto              xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar           lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarCapacitor     xmlCapacitor;   // Capacitor
   _di_IXMLTEqBarCapacitorList  lstCapacitor;   // Lista de Capacitores

   try{//determina lista de Capacitores da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoCAPACITOR);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Capacitores
      if ((lstCapacitor = lstEqBar->Get_Capacitor()) == NULL) return(false);
      //loop p/ todos Capacitores
      for (int n = 0; n < lisEQP->Count; n++)
         {
         capacitor = (VTCapacitor*)lisEQP->Items[n];
         //insere um novo Capacitor
         if ((xmlCapacitor = lstCapacitor->Add())       == NULL) return(false);
         if ((xmlEqpto     = xmlCapacitor->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Capacitor
         if (!MontaDadosIdent(capacitor, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Capacitor
         if (!MontaDadosCoord(capacitor, xmlEqpto->Get_coordId()->Add())) return(false);
         //insere atributos do Capacitor
         xmlCapacitor->Set_intTpoLig(capacitor->ligacao);
         xmlCapacitor->Set_dblPotRtv(capacitor->q);
         xmlCapacitor->Set_dblTnsNom(capacitor->vnom);

         // Salva as medições
         if (!MontaDadosMedicao(rede, capacitor, xmlEqpto)) return(false);
         }

      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCapserie(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTCapserie                       *capserie;
   _di_IXMLTId                      xmlId;
   _di_IXMLTCnjEqLig                lstEqLig;
   _di_IXMLTEqLigCapacitorSerieList  lstCapacitorSerie;    // Conjunto de capacitores série
   _di_IXMLTEqLigCapacitorSerie     xmlCapacitorSerie;    // Conjunto de capacitores série
   _di_IXMLTEqpto                   xmlEqpto;             // Eqpto
   _di_IXMLTZ                       xmlZ;

   try{//loop p/ todos Capacitores Serie da Rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoCAPSERIE);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstCapacitorSerie = lstEqLig->Get_CapacitorSerie();

      //loop p/ todos Capacitores Serie
      for (int n = 0; n < lisEQP->Count; n++)
         {
         capserie = (VTCapserie*)lisEQP->Items[n];

         //insere um novo Capacitor Serie
         if ((xmlCapacitorSerie = lstCapacitorSerie->Add())       == NULL) return(false);
         if ((xmlEqpto          = xmlCapacitorSerie->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação do Capacitor Serie
         if (!MontaDadosIdent(capserie, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Capacitor Serie
         if (!MontaDadosCoord(capserie, xmlEqpto->Get_coordId())) return(false);
         //insere atributos do Capacitor Serie
         xmlCapacitorSerie->Set_dblTnsNom(capserie->vnom);
         xmlCapacitorSerie->Set_dblPotNom(capserie->snom);

         if ((xmlZ = xmlCapacitorSerie->Get_z()) == NULL) return(false);
         xmlZ->Set_dblR(capserie->z.r);
         xmlZ->Set_dblX(capserie->z.x);

         // Salva as medições
         if (!MontaDadosMedicao(rede, capserie, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCarga(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTCarga                *carga;
   _di_IXMLTEqpto         xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar      lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarCarga    xmlCarga;       // Carga
   _di_IXMLTEqBarCargaList lstCarga;       // Lista de Cargas
   _di_IXMLTPatamPropList  lstPatamar;     // Lista de patamares
   VTFases*               fases;

   try{//determina lista de Cargas da Rede
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);

      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoCARGA);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Cargas
      if ((lstCarga = lstEqBar->Get_Carga()) == NULL) return(false);

      //loop p/ todas Cargas da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         carga = (VTCarga*)lisEQP->Items[n];

         // Cargas IP são um caso especial, executadas em um método próprio
         if (carga->classe->Tag == consIP) continue;

         //insere uma nova Carga
         if ((xmlCarga = lstCarga->Add())       == NULL) return(false);
         if ((xmlEqpto = xmlCarga->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação da Carga
         if (!MontaDadosIdent(carga, xmlEqpto)) return(false);
         //insere atributos de coordenadas da Carga
         if (!MontaDadosCoord(carga, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos da Carga
         xmlCarga->Set_intTpoCons(carga->classe->Tag);
         xmlCarga->Set_intTpoModCrg(carga->ModeloCarga);
         xmlCarga->Set_dblEne(carga->Energia_mwhmes);
         xmlCarga->Set_strFases(fases->AsStrABCN[carga->Fases]);

         if (! carga->curva->Tipica)
            {//Carga c/ curva de demanda própria
            if ((lstPatamar = xmlCarga->Get_Patamar()) == NULL) return(false);
            //verifica se Carga com demanda trifásica ou por fase
            if (carga->curva->NumeroValores == nvCURVA_CAR)
               {
               if (!MontaDadosCurva(carga->curva, lstPatamar)) return(false);
               }
            else if (carga->curva->NumeroValores == nvCURVA_RRCAR)
               {
               if (!MontaDadosCurvaCargaEqv(carga->curva, lstPatamar)) return(false);
               }
            }
         // Salva as medições
         if (!MontaDadosMedicao(rede, carga, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCarga(VTReducao *reducao, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTCarga                *carga;
   _di_IXMLTEqpto         xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar      lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarCarga    xmlCarga;       // Carga
   _di_IXMLTEqBarCargaList lstCarga;       // Lista de Cargas
   _di_IXMLTPatamPropList  lstPatamar;     // Lista de patamares
   VTFases*               fases;

   try{//determina lista de Cargas da Rede
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);

      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoCARGA);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto XmlRede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Cargas
      if ((lstCarga = lstEqBar->Get_Carga()) == NULL) return(false);
      //loop p/ todas Cargas da Reducao
      for (int n = 0; n < lisEQP->Count; n++)
         {
         carga = (VTCarga*)lisEQP->Items[n];
         //insere uma nova Carga
         if ((xmlCarga = lstCarga->Add())       == NULL) return(false);
         if ((xmlEqpto = xmlCarga->Get_eqbar()) == NULL) return(false);
         //insere atributos de identificação da Carga
         if (!MontaDadosIdent(carga, xmlEqpto)) return(false);
         //insere atributos de coordenadas da Carga
         if (!MontaDadosCoord(carga, xmlEqpto->Get_coordId()->Add())) return(false);
         //insere atributos da Carga
         xmlCarga->Set_intTpoCons(carga->classe->Tag);
         xmlCarga->Set_intTpoModCrg(carga->ModeloCarga);
         xmlCarga->Set_dblEne(carga->Energia_mwhmes);
         xmlCarga->Set_strFases(fases->AsStrABCN[carga->Fases]);
         //carga equivalente possui demanda própria
         if (carga->curva != NULL)
            {
            if ((lstPatamar = xmlCarga->Get_Patamar()) == NULL) return(false);
            if (!MontaDadosCurvaCargaEqv(carga->curva, lstPatamar))   return(false);
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCargaIP(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTCarga                  *carga;
   _di_IXMLTEqpto           xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar        lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarCargaIP    xmlCargaIP;       // Carga
   _di_IXMLTEqBarCargaIPList lstCargaIP;       // Lista de Cargas
   VTFases*                 fases;

   try{//determina lista de Cargas da Rede
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);

      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoCARGA);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Cargas
      if ((lstCargaIP = lstEqBar->Get_CargaIP()) == NULL) return(false);

      //loop p/ todas CargasIP da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         carga = (VTCarga*)lisEQP->Items[n];

         // Ignora se não for uma carga IP
         if (carga->classe->Tag != consIP) continue;

         //insere uma nova Carga
         if ((xmlCargaIP = lstCargaIP->Add())     == NULL) return(false);
         if ((xmlEqpto = xmlCargaIP->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação da Carga
         if (!MontaDadosIdent(carga, xmlEqpto)) return(false);
         //insere atributos de coordenadas da Carga
         if (!MontaDadosCoord(carga, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos da Carga
         xmlCargaIP->Set_intNumLamp(carga->IP_NumLamp);
         xmlCargaIP->Set_dblPotAtv(carga->IP_Pot_kw);
         xmlCargaIP->Set_dblFtrPot(carga->IP_Fatpot);
         xmlCargaIP->Set_strFases(fases->AsStrABCN[carga->Fases]);

         // Salva as medições
         if (!MontaDadosMedicao(rede, carga, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoChave(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTChave                 *chave;
   _di_IXMLTId             xmlId;
   _di_IXMLTCnjEqLig       lstEqLig;
   _di_IXMLTEqLigChaveList  lstChave;    // Conjunto de Chaves
   _di_IXMLTEqLigChave     xmlChave;    // Chave
   _di_IXMLTEqpto          xmlEqpto;    // Eqpto

   try{//determina lista de Chaves da Rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoCHAVE);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstChave = lstEqLig->Get_Chave();

      //loop p/ todas Chaves
      for (int n = 0; n < lisEQP->Count; n++)
         {
         chave = (VTChave*)lisEQP->Items[n];

         //insere uma nova Chave
         if ((xmlChave = lstChave->Add())       == NULL) return(false);
         if ((xmlEqpto = xmlChave->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação da Chave
         if (!MontaDadosIdent(chave, xmlEqpto)) return(false);
         //insere atributos de coordenadas da Chave
         if (!MontaDadosCoord(chave, xmlEqpto->Get_coordId())) return(false);
         //insere atributos da Chave
         xmlChave->Set_strTpoChv(chave->TipoChave->Codigo);
         xmlChave->Set_intEst(chave->Estado);
         xmlChave->Set_bolBlq(false);
         xmlChave->Set_dblCorNom(chave->Inom_amp);

         // Salva as medições
         if (!MontaDadosMedicao(rede, chave, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoConcessionaria(void)
   {
   //variáveis locais
   _di_IXMLTId xmlId;                 // identificador de uso geral (somente ponteiro)
   AnsiString  strEmpresa;
   AnsiString  strEmpresaDescricao;
   VTLicenca*  licenca;
   VTLicencas* licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

   try{
      // DEPOOPS: colocar namespace????
      // WideString     strNameSpace = "SinapModeloRede_v20.xsd";
      xmlConcessAux->Set_strVer("2.1");

      // Como agora a versão está escrita no arquivo (Set_strVer acima), não há
      // necessidade de indicar o namespace em cada campo. O XML fica mais "limpo".
      // xmlConcessAux->DeclareNamespace("", strNameSpace);
      // TALVEZ SEJA ÚTIL NA IMPORTAÇÃO, PARA VERIFICAÇÃO DO ARQUIVO ("bem formado").

      // Passa para o objeto XML os dados da concessionária (aqui colocados como fixos).
      xmlId = xmlConcessAux->Get_id();
      xmlId->Set_intId(1);

      // Descobre o nome da empresa
      strEmpresa          = "";
      strEmpresaDescricao = "";
      licenca = licencas->ExisteLicenca();
      if (licenca)
         {
         strEmpresa = licenca->EmpresaStr;
         }

      xmlId->Set_strCod(strEmpresa);
      xmlId->Set_strDsc(strEmpresaDescricao);
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicas(void)
   {
   if (!MontaConjuntoCurvasTipicasCarga())       return(false);
   if (!MontaConjuntoCurvasTipicasCargaIP())     return(false);
   if (!MontaConjuntoCurvasTipicasCapacitor())   return(false);
   if (!MontaConjuntoCurvasTipicasGeradorFoto()) return(false);
   if (!MontaConjuntoCurvasTipicasReator())      return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicasCarga(void)
   {
   VTCurva*                 curva  = NULL;
   TList*                   lisCur = NULL;
   _di_IXMLTCurvaTipicaList  xmlCurvaTipicaList;
   VTCurvas*                curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!xmlConcessAux)                                        return(false);
   if (curvas == NULL)                                        return(false);
   if ((lisCur = curvas->ListaCurvaTipicaCarga()) == NULL)    return(false);
   xmlCurvaTipicaList = xmlConcessAux->Get_CurvaTipicaCarga();
   if (!xmlCurvaTipicaList)                                    return(false);

   // Loop das curvas
   for (int i = 0; i < lisCur->Count; i++)
      {
      if ((curva = (VTCurva*)lisCur->Items[i]) == NULL)       return(false);

      if (!MontaDadosCurvaTip(curva, xmlCurvaTipicaList, true)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicasCargaIP(void)
   {
   VTCurva*                 curva  = NULL;
   TList*                   lisCur = NULL;
   _di_IXMLTCurvaTipicaList  xmlCurvaTipicaList;
   VTCurvas*                curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!xmlConcessAux)                                        return(false);
   if (curvas == NULL)                                        return(false);
   if ((lisCur = curvas->ListaCurvaTipicaIP()) == NULL)       return(false);
   xmlCurvaTipicaList = xmlConcessAux->Get_CurvaTipicaCargaIP();
   if (!xmlCurvaTipicaList)                                     return(false);

   // Loop das curvas
   for (int i = 0; i < lisCur->Count; i++)
      {
      if ((curva = (VTCurva*)lisCur->Items[i]) == NULL)         return(false);

      if (!MontaDadosCurvaTip(curva, xmlCurvaTipicaList, true)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicasCapacitor(void)
   {
   VTCurva*                 curva  = NULL;
   TList*                   lisCur = NULL;
   _di_IXMLTCurvaTipicaList  xmlCurvaTipicaList;
   VTCurvas*                curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!xmlConcessAux)                                          return(false);
   if (curvas == NULL)                                          return(false);
   if ((lisCur = curvas->ListaCurvaTipicaCapacitor()) == NULL)  return(false);
   xmlCurvaTipicaList = xmlConcessAux->Get_CurvaTipicaCapacitor();
   if (!xmlCurvaTipicaList)                                      return(false);

   // Loop das curvas
   for (int i = 0; i < lisCur->Count; i++)
      {
      if ((curva = (VTCurva*)lisCur->Items[i]) == NULL)         return(false);

      if (!MontaDadosCurvaTip(curva, xmlCurvaTipicaList, false)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicasGeradorFoto(void)
   {
   VTCurva*                 curva  = NULL;
   TList*                   lisCur = NULL;
   _di_IXMLTCurvaTipicaList  xmlCurvaTipicaList;
   VTCurvas*                curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!xmlConcessAux)                                                    return(false);
   if (curvas == NULL)                                                    return(false);
   // OOPS. É isso?
   if ((lisCur = curvas->ListaCurvaTipicaGeradorFotovoltaico()) == NULL)  return(false);
   // OOPS
   // Modificar esse nome para Get_CurvaTipicaGeradorFoto ???
   xmlCurvaTipicaList = xmlConcessAux->Get_CurvaTipicaGeradorFoto();
   if (!xmlCurvaTipicaList)                                                return(false);

   // Loop das curvas
   for (int i = 0; i < lisCur->Count; i++)
      {
      if ((curva = (VTCurva*)lisCur->Items[i]) == NULL)                   return(false);

      if (!MontaDadosCurvaTip(curva, xmlCurvaTipicaList, false))           return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoCurvasTipicasReator(void)
   {
   VTCurva*                 curva  = NULL;
   TList*                   lisCur = NULL;
   _di_IXMLTCurvaTipicaList  xmlCurvaTipicaList;
   VTCurvas*                curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Evita explosões
   if (!xmlConcessAux)                                          return(false);
   if (curvas == NULL)                                          return(false);
   if ((lisCur = curvas->ListaCurvaTipicaReator()) == NULL)     return(false);
   xmlCurvaTipicaList = xmlConcessAux->Get_CurvaTipicaReator();
   if (!xmlCurvaTipicaList)                                      return(false);

   // Loop das curvas
   for (int i = 0; i < lisCur->Count; i++)
      {
      if ((curva = (VTCurva*)lisCur->Items[i]) == NULL)         return(false);

      if (!MontaDadosCurvaTip(curva, xmlCurvaTipicaList, false))        return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoFiltro(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTFiltro                *filtro;
   _di_IXMLTEqpto          xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar       lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarFiltro    xmlFiltro;      // Carga
   _di_IXMLTEqBarFiltroList lstFiltro;      // Lista de Cargas
   _di_IXMLTZ              xmlZ;

   try{//determina lista de Filtros da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoFILTRO);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Filtros
      if ((lstFiltro = lstEqBar->Get_Filtro()) == NULL) return(false);

      //loop p/ todos Filtros da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         filtro = (VTFiltro*)lisEQP->Items[n];

         //insere uma nova Carga
         if ((xmlFiltro = lstFiltro->Add())       == NULL) return(false);
         if ((xmlEqpto  = xmlFiltro->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Filtro
         if (!MontaDadosIdent(filtro, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Filtro
         if (!MontaDadosCoord(filtro, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do Filtro
         xmlFiltro->Set_dblTnsNom(filtro->vnom);
         xmlFiltro->Set_dblPotNom(filtro->snom);

         if ((xmlZ = xmlFiltro->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(filtro->z0.r);
         xmlZ->Set_dblX(filtro->z0.x);

         // Salva as medições
         if (!MontaDadosMedicao(rede, filtro, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoGerador(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTGerador                *gerador;
   _di_IXMLTEqpto           xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar        lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarGerador    xmlGerador;     // Gerador
   _di_IXMLTEqBarGeradorList lstGerador;     // Lista de Geradores
   _di_IXMLTZseq            xmlZSeq;        // Impedância de seqüência
   _di_IXMLTZ               xmlZ;
   _di_IXMLTPatamPropList    lstPatamar;     // Lista de patamares

   try{//determina lista de Geradores da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoGERADOR);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Geradores
      if ((lstGerador = lstEqBar->Get_Gerador()) == NULL) return(false);

      //loop p/ todos Geradores da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         gerador = (VTGerador*)lisEQP->Items[n];

         //insere um novo Gerador
         if ((xmlGerador = lstGerador->Add())       == NULL) return(false);
         if ((xmlEqpto   = xmlGerador->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Gerador
         if (!MontaDadosIdent(gerador, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Gerador
         if (!MontaDadosCoord(gerador, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do Gerador
         xmlGerador->Set_intTpoLig(gerador->ligacao);
         xmlGerador->Set_intTpoSup(gerador->tiposup);
         xmlGerador->Set_dblTnsNom(gerador->vnom);
         xmlGerador->Set_dblPotNom(gerador->snom);
         xmlGerador->Set_dblPotMax(gerador->smax);
         xmlGerador->Set_dblFtrPotLim(gerador->fplim);

         if ((xmlZSeq = xmlGerador->Get_zSeq()) == NULL) return(false);
         xmlZ = xmlZSeq->Get_z0();
         xmlZ->Set_dblR(gerador->z0.r);
         xmlZ->Set_dblX(gerador->z0.x);
         xmlZ = xmlZSeq->Get_z1();
         xmlZ->Set_dblR(gerador->z1.r);
         xmlZ->Set_dblX(gerador->z1.x);
         
         // Exporta os patamares
         if ((gerador->curva) != NULL)
             {
             if ((lstPatamar = xmlGerador->Get_Patamar()) == NULL) return(false);
             if (!MontaDadosCurva(gerador->curva, lstPatamar)) return(false);
             }

         // Salva as medições
         if (!MontaDadosMedicao(rede, gerador, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoMedidor(VTRede *rede, _di_IXMLTRede xmlRede)
    {
   VTMedidor                *medidor;
   _di_IXMLTEqBarMedidor    xmlMedidor;      // Medidor
   _di_IXMLTEqBarMedidorList lstMedidor;      // Lista de Medidores
   _di_IXMLTEqpto           xmlEqpto;        // Eqpto
   _di_IXMLTCnjEqBar        lstEqBar;        // Lista de EqBar

   try{//determina lista de Medidores da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoMEDIDOR);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Medidores
      if ((lstMedidor = lstEqBar->Get_Medidor()) == NULL) return(false);

      //loop p/ todos Medidores da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         medidor = (VTMedidor*)lisEQP->Items[n];

         //insere um novo Medidor
         if ((xmlMedidor = lstMedidor->Add()) == NULL) return(false);
         if ((xmlEqpto  = xmlMedidor->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Medidor
         if (!MontaDadosIdent(medidor, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Medidor
         if (!MontaDadosCoord(medidor, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do Medidor
         // Não há atributos...

         // Salva as medições de tensão da Barra do medidor
         if (!MontaDadosMedicao(rede, medidor->pbarra, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
    } // MontaConjuntoMedidor

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoMutua(VTRede *rede, _di_IXMLTRede xmlRede)
    {
    VTMutua                *mutua;
    _di_IXMLTMutuaList      lstMutua;      // Lista de Mutuas
    _di_IXMLTMutua         xmlMutua;      // Reator
    _di_IXMLTMutuaTreList   cnjMutuaTre;   // Trechos da mútua
    _di_IXMLTMutuaTre      xmlMutuaTre;   // Trecho da mútua
    _di_IXMLTId            xmlId;        // Id do equipamento
    _di_IXMLTZseq          xmlZSeq;      // Impedância de seqüência
    _di_IXMLTZ             xmlZ;         // Impedância
    TList*                 ptrListMutua;  // Lista de objetos mutua
    int                    intSentido;

    try
        {//determina lista de Mutuaes da Rede
        if ((ptrListMutua = rede->LisMutua()) == NULL) return(true);  // oops: deve retornar false?
        if (ptrListMutua->Count == 0) return(true);

        //obtém conjunto de mútuas do conjunto Rede
        if ((lstMutua = xmlRede->Get_Mutua()) == NULL) return(false);

        //loop p/ todos Mutuaes da Rede
        for (int n = 0; n < ptrListMutua->Count; n++)
            {
            mutua = (VTMutua*)ptrListMutua->Items[n];

            //insere uma nova Mutua
            if ((xmlMutua = lstMutua->Add()) == NULL) return(false);
            if ((xmlId    = xmlMutua->Get_id()) == NULL) return(false);

            //insere atributos do Mutua
            xmlId->Set_intId(mutua->Id);
            xmlId->Set_strCod(mutua->Codigo);
            xmlId->Set_strDsc("");
            xmlId->Set_strExtId(mutua->Extern_id);

            xmlMutua->Set_intRedeId(rede->Id);

            cnjMutuaTre = xmlMutua->Get_MutuaTre();

            xmlMutuaTre = cnjMutuaTre->Add();
            xmlMutuaTre->Set_intId(mutua->ptre1->Id);
            //intSentido = (mutua->pbar_tre1 == mutua->ptre1->pbarra1) ? 12 : 21;
            intSentido = mutua->pbar_tre1->Id;
            xmlMutuaTre->Set_intSentido(intSentido);
            xmlMutuaTre->Set_dblPos(mutua->postre1);

            xmlMutuaTre = cnjMutuaTre->Add();
            xmlMutuaTre->Set_intId(mutua->ptre2->Id);
            //intSentido = (mutua->pbar_tre2 == mutua->ptre2->pbarra1) ? 12 : 21;
            intSentido = mutua->pbar_tre2->Id;
            xmlMutuaTre->Set_intSentido(intSentido);
            xmlMutuaTre->Set_dblPos(mutua->postre2);

            xmlZSeq  = xmlMutua->Get_zSeq();
            xmlZ     = xmlZSeq->Get_z0();
            xmlZ->Set_dblR(mutua->z0.r);
            xmlZ->Set_dblX(mutua->z0.x);
            xmlZ     = xmlZSeq->Get_z1();
            xmlZ->Set_dblR(mutua->z1.r);
            xmlZ->Set_dblX(mutua->z1.x);
           }
        }
    catch(Exception &e)
        {
        return(false);
        }

    return(true);
    } // MontaConjuntoMutua

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoReator(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTReator                 *reator;
   _di_IXMLTEqpto           xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar        lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarReator     xmlReator;      // Reator
   _di_IXMLTEqBarReatorList  lstReator;      // Lista de Reatores

   try{//determina lista de Reatores da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoREATOR);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Reatores
      if ((lstReator = lstEqBar->Get_Reator()) == NULL) return(false);

      //loop p/ todos Reatores da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         reator = (VTReator*)lisEQP->Items[n];

         //insere um novo Reator
         if ((xmlReator = lstReator->Add())       == NULL) return(false);
         if ((xmlEqpto  = xmlReator->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Reator
         if (!MontaDadosIdent(reator, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Reator
         if (!MontaDadosCoord(reator, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do Reator
         xmlReator->Set_intTpoLig(reator->ligacao);
         xmlReator->Set_dblTnsNom(reator->vnom);
         xmlReator->Set_dblPotRtv(reator->q);

         // Salva as medições
         if (!MontaDadosMedicao(rede, reator, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoReducao(void)
   {
   //variáveis locais
   _di_IXMLTId      xmlId;
   _di_IXMLTRede    xmlRede;
   _di_IXMLTRedeList xmlRedeList;
   VTMRede          *mrede;
   VTRede           *rede;
   VTReducao        *reducao;
   VTRedes          *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList            *lisMR = redes->LisMRede();

   try{//obtém objeto xmlListRede
      xmlRedeList = xmlConcessAux->Get_Rede();
      //loop p/ todo objeto Rede
      for (int n = 0; n < lisMR->Count; n++)
         {
         mrede = (VTMRede*)lisMR->Items[n];
         //verifica se a Rede está carregada
         if (mrede->Carregada) continue;
         //obtém Reducao única da MRede
         reducao = (VTReducao*)((mrede->LisReducao())->First());
         //TEMPORÁRIO: verifica se a Reducao possui um único Eqbar
         if ((reducao->LisLigacao())->Count > 0) continue;
         if (reducao->NumeroDeEqbar(eqptoINDEF) != 1) continue;
         //insere um objeto xmlRede em
         xmlRede = xmlRedeList->Add();
         //insere TipoRede
         xmlRede->Set_strTpo("Equivalente");
         if (! MontaDadosIdent(reducao, xmlRede->Get_id())) return(false);
         //insere dados das Cargas equivalentes
         if (! MontaConjuntoCarga(reducao, xmlRede))      return(false);
         //insere dados dos Suprimentos equivalentes
         if (! MontaConjuntoSuprimento(reducao, xmlRede))      return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoRegulador(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTRegulador                 *regulador;
   _di_IXMLTId                 xmlId;
   _di_IXMLTCnjEqLig           lstEqLig;
   _di_IXMLTEqLigReguladorList  lstRegulador;  // Conjunto de reguladores
   _di_IXMLTEqLigRegulador     xmlRegulador;  // Regulador
   _di_IXMLTEqpto              xmlEqpto;      // Eqpto
   _di_IXMLTCoordId            xmlCoordId;    // Coordenada identificada (Barras)
   _di_IXMLTCoord              xmlCoord;      // Coordenada
   _di_IXMLTZseq               xmlZSeq;       // Impedância de seqüência
   _di_IXMLTZ                  xmlZ;          // Impedância
   _di_IXMLTRegTapFix          xmlRegTapFix;  // Estrutura que armazena dados específicos de cada tipo de regulador
   _di_IXMLTRegTapAjs          xmlRegTapAjs;  // Estrutura que armazena dados específicos de cada tipo de regulador

   try{//determina lista de Reguladores da Rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoREGULADOR);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstRegulador = lstEqLig->Get_Regulador();

      //loop p/ todas Reguladores
      for (int n = 0; n < lisEQP->Count; n++)
         {
         regulador = (VTRegulador*)lisEQP->Items[n];

         //insere uma nova Regulador
         if ((xmlRegulador = lstRegulador->Add())       == NULL) return(false);
         if ((xmlEqpto     = xmlRegulador->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação do Reator
         if (!MontaDadosIdent(regulador, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Reator
         if (!MontaDadosCoord(regulador, xmlEqpto->Get_coordId())) return(false);

         //insere atributos de identificação da Regulador
         xmlRegulador->Set_intTpoLig(regulador->ligacao);
         xmlRegulador->Set_dblPotNom(regulador->snom);
         xmlRegulador->Set_bolBypass(regulador->by_pass);
         xmlRegulador->Set_dblFxaVar(regulador->var_tensao);
         xmlRegulador->Set_intPasNum(regulador->num_passo);
//         xmlRegulador->Set_bolSntRev(regulador->sentido.reversivel);         /* TODO : reg */
         //impedâncias de sequência
         xmlZSeq           = xmlRegulador->Get_zSeq();
         xmlZ              = xmlZSeq->Get_z0();
         xmlZ->Set_dblR(regulador->z0.r);
         xmlZ->Set_dblX(regulador->z0.x);
         xmlZ              = xmlZSeq->Get_z1();
         xmlZ->Set_dblR(regulador->z1.r);
         xmlZ->Set_dblX(regulador->z1.x);
         //verifica se o Regulador tem fluxo reversível
//		 if ((! regulador->sentido.reversivel)&&(regulador->sentido.bar_ref))    /* TODO : reg */
		 if (regulador->sentido.bar_ref)
			 {
			 if ((xmlCoordId = xmlRegulador->Get_coordBarRefSnt()) == NULL) return(false);
             xmlCoordId->Set_strCod(IntToStr(regulador->sentido.bar_ref->Id));

             if ((xmlCoord = xmlCoordId->Get_coordUTM()) == NULL) return(false);
             xmlCoord->Set_dblX(regulador->sentido.bar_ref->utm.x);
             xmlCoord->Set_dblY(regulador->sentido.bar_ref->utm.y);
             if ((xmlCoord = xmlCoordId->Get_coordESQ1()) == NULL) return(false);
             xmlCoord->Set_dblX(regulador->sentido.bar_ref->esq.x1);
             xmlCoord->Set_dblY(regulador->sentido.bar_ref->esq.y1);
             if ((xmlCoord = xmlCoordId->Get_coordESQ2()) == NULL) return(false);
             xmlCoord->Set_dblX(regulador->sentido.bar_ref->esq.x2);
             xmlCoord->Set_dblY(regulador->sentido.bar_ref->esq.y2);
             }

		 if ((regulador->modelo != reguENERQCT) && (regulador->modelo != reguFIXO))
             {//Regulador com tap ajustável
             // Ajustável
             if ((xmlRegTapAjs = xmlRegulador->Get_regTapAjs()) == NULL) return(false);

             xmlRegTapAjs->Set_dblTnsAlvo(regulador->cfg_auto.v_pu);
             if (regulador->cfg_auto.bar_ref)
                 {
                 if ((xmlCoordId = xmlRegTapAjs->Get_coordBarRefCfg()) == NULL) return(false);
                 xmlCoordId->Set_strCod(IntToStr(regulador->cfg_auto.bar_ref->Id));

                 if ((xmlCoord = xmlCoordId->Get_coordUTM()) == NULL) return(false);
                 xmlCoord->Set_dblX(regulador->cfg_auto.bar_ref->utm.x);
                 xmlCoord->Set_dblY(regulador->cfg_auto.bar_ref->utm.y);
                 if ((xmlCoord = xmlCoordId->Get_coordESQ1()) == NULL) return(false);
                 xmlCoord->Set_dblX(regulador->cfg_auto.bar_ref->esq.x1);
                 xmlCoord->Set_dblY(regulador->cfg_auto.bar_ref->esq.y1);
                 if ((xmlCoord = xmlCoordId->Get_coordESQ2()) == NULL) return(false);
                 xmlCoord->Set_dblX(regulador->cfg_auto.bar_ref->esq.x2);
                 xmlCoord->Set_dblY(regulador->cfg_auto.bar_ref->esq.y2);
                 }
             }
         else
             {//Regulador com tap fixo
             // Fixo
             if ((xmlRegTapFix = xmlRegulador->Get_regTapFix()) == NULL) return(false);

             xmlRegTapFix->Set_intPasSel(regulador->cfg_fixo.passo);
             }

         // Salva as medições
         if (!MontaDadosMedicao(rede, regulador, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoSuporte(void)
   {
   //variáveis locais
   _di_IXMLTSuporteList          xmlSuporteList;
   _di_IXMLTSuporte             xmlSuporte;
   _di_IXMLTId                  xmlId;
   _di_IXMLTSuportePtoFixList    xmlPtoFixList;
   _di_IXMLTSuportePtoFix       xmlPtoFix;
   _di_IXMLTCoord               xmlCoord;
   TList                        *lisPTO;
   VTPtoFixacao                 *pto_fixacao;
   VTSuporte                    *suporte;
   VTArranjos                   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList                        *lisSUPORTE  = arranjos->LisSuporte();

   try{//obtém objeto xmlSuporteList
      if ((xmlSuporteList = xmlConcessAux->Get_Suporte()) == NULL) return(false);
      //loop p/ todos objeto Suporte
      for (int n = 0; n < lisSUPORTE->Count; n++)
         {
         suporte = (VTSuporte*)lisSUPORTE->Items[n];
         //descarta Suporte com código não definido
         if (suporte->Codigo.IsEmpty()) continue;
         //insere um novo objeto xmlSuporte em xmlSuporteList
         if ((xmlSuporte   = xmlSuporteList->Add())     == NULL) return(false);
         if ((xmlId        = xmlSuporte->Get_id())     == NULL) return(false);
         if ((xmlPtoFixList = xmlSuporte->Get_PtoFix()) == NULL) return(false);
         //copia dados do cabo
         xmlId->Set_intId(suporte->Id);
         xmlId->Set_strCod(suporte->Codigo);
         xmlSuporte->Set_strTpo(suporte->TipoDeSuporteAsString());
         xmlSuporte->Set_dblTnsMin(suporte->Vmin_KV);
         xmlSuporte->Set_dblTnsMax(suporte->Vmax_KV);
         //copia pontos de fixação
         lisPTO = suporte->LisPtoFixacao();
         for (int np = 0; np < lisPTO->Count; np++)
            {
            pto_fixacao = (VTPtoFixacao*)lisPTO->Items[np];
            //insere nova xmlCoord em xmlCoordList
            xmlPtoFix = xmlPtoFixList->Add();
            //ID do PtoFixação (número sequencial a partir de 0 que equivale ao índice do PtoFixacao na lista)
            xmlPtoFix->Set_intId(np);
            if ((xmlCoord = xmlPtoFix->Get_coordPnt()) == NULL) return(false);
            xmlCoord->Set_dblX(pto_fixacao->coord.x);
            xmlCoord->Set_dblY(pto_fixacao->coord.y);
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoSuprimento(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTSuprimento                 *suprimento;
   _di_IXMLTEqpto               xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar            lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarSuprimento     xmlSuprimento;  // Suprimento
   _di_IXMLTEqBarSuprimentoList  lstSuprimento;  // Lista de Suprimentoes
   _di_IXMLTZseq                xmlZeqv;        // Impedância de seqüência
   _di_IXMLTZ                   xmlZ;           // Impedância
   _di_IXMLTPotCpx              xmlPotCpx;      // Impedância
   _di_IXMLTPatamPropList        lstPatamar;     // Lista de patamares
   double                       vnom_kv;

   try{//determina lista de Suprimentoes da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Suprimentoes
      if ((lstSuprimento = lstEqBar->Get_Suprimento()) == NULL) return(false);

      //loop p/ todos Suprimentoes da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         suprimento = (VTSuprimento*)lisEQP->Items[n];

         //insere um novo Suprimento
         if ((xmlSuprimento = lstSuprimento->Add())       == NULL) return(false);
         if ((xmlEqpto      = xmlSuprimento->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Suprimento
         if (!MontaDadosIdent(suprimento, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Suprimento
         if (!MontaDadosCoord(suprimento, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do Suprimento
         //tipo de suprimento
         xmlSuprimento->Set_intTpoSup(suprimento->tiposup);
         //tensão nomimanl (proteção p/ versões anteriores sem Vnom p/ Suprimento)
         vnom_kv = (IsDoubleZero(suprimento->vnom)) ? suprimento->pbarra->vnom : suprimento->vnom;
         xmlSuprimento->Set_dblTnsNom(vnom_kv);
         //potência máxima
         xmlSuprimento->Set_dblPotMax(suprimento->smax);
         //potência de curto trifásico
         if ((xmlPotCpx = xmlSuprimento->Get_cpxPotCC3f()) == NULL) return(false);
         xmlPotCpx->Set_dblPotAtv(suprimento->pcc_3f.p);
         xmlPotCpx->Set_dblPotRtv(suprimento->pcc_3f.q);
         //potência de curto fase/terra
         if ((xmlPotCpx = xmlSuprimento->Get_cpxPotCCFt()) == NULL) return(false);
         xmlPotCpx->Set_dblPotAtv(suprimento->pcc_ft.p);
         xmlPotCpx->Set_dblPotRtv(suprimento->pcc_ft.q);
         //impedância equivalente: Z0 e Z1
         if ((xmlZeqv = xmlSuprimento->Get_zEqv()) == NULL) return(false);
         if ((xmlZ = xmlZeqv->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(suprimento->zeq0.r);
         xmlZ->Set_dblX(suprimento->zeq0.x);
         if ((xmlZ = xmlZeqv->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(suprimento->zeq1.r);
         xmlZ->Set_dblX(suprimento->zeq1.x);
         //curva de fornecimento
         if ((suprimento->curva) != NULL)
            {
            if ((lstPatamar = xmlSuprimento->Get_Patamar()) == NULL) return(false);
            if (!MontaDadosCurva(suprimento->curva, lstPatamar)) return(false);
            }
         // Salva as medições
         if (!MontaDadosMedicao(rede, suprimento, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoSuprimento(VTReducao *reducao, _di_IXMLTRede xmlRede)
   {
   VTSuprimento                 *suprimento;
   _di_IXMLTEqpto               xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar            lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarSuprimento     xmlSuprimento;  // Suprimento
   _di_IXMLTEqBarSuprimentoList  lstSuprimento;  // Lista de Suprimentoes
   _di_IXMLTZseq                xmlZeqv;        // Impedância de seqüência
   _di_IXMLTZ                   xmlZ;           // Impedância
   _di_IXMLTPotCpx              xmlPotCpx;      // Impedância
   _di_IXMLTPatamPropList        lstPatamar;     // Lista de patamares
   double                       vnom_kv;

   try{//determina lista de Suprimentoes da Rede
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de Suprimentoes
      if ((lstSuprimento = lstEqBar->Get_Suprimento()) == NULL) return(false);

      //loop p/ todos Suprimentoes da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         suprimento = (VTSuprimento*)lisEQP->Items[n];

         //insere um novo Suprimento
         if ((xmlSuprimento = lstSuprimento->Add())       == NULL) return(false);
         if ((xmlEqpto      = xmlSuprimento->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do Suprimento
         if (!MontaDadosIdent(suprimento, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Suprimento
         if (!MontaDadosCoord(suprimento, xmlEqpto->Get_coordId()->Add())) return(false);
         //insere atributos do Suprimento
         //tipo de suprimento
         xmlSuprimento->Set_intTpoSup(suprimento->tiposup);
         //tensão nomimanl (proteção p/ versões anteriores sem Vnom p/ Suprimento)
         vnom_kv = (IsDoubleZero(suprimento->vnom)) ? suprimento->pbarra->vnom : suprimento->vnom;
         xmlSuprimento->Set_dblTnsNom(vnom_kv);
         //potência máxima
         xmlSuprimento->Set_dblPotMax(suprimento->smax);
         //potência de curto trifásico
         if ((xmlPotCpx = xmlSuprimento->Get_cpxPotCC3f()) == NULL) return(false);
         xmlPotCpx->Set_dblPotAtv(suprimento->pcc_3f.p);
         xmlPotCpx->Set_dblPotRtv(suprimento->pcc_3f.q);
         //potência de curto fase/terra
         if ((xmlPotCpx = xmlSuprimento->Get_cpxPotCCFt()) == NULL) return(false);
         xmlPotCpx->Set_dblPotAtv(suprimento->pcc_ft.p);
         xmlPotCpx->Set_dblPotRtv(suprimento->pcc_ft.q);
         //impedância equivalente: Z0 e Z1
         if ((xmlZeqv = xmlSuprimento->Get_zEqv()) == NULL) return(false);
         if ((xmlZ = xmlZeqv->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(suprimento->zeq0.r);
         xmlZ->Set_dblX(suprimento->zeq0.x);
         if ((xmlZ = xmlZeqv->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(suprimento->zeq1.r);
         xmlZ->Set_dblX(suprimento->zeq1.x);
         //curva de fornecimento
         if ((suprimento->curva) != NULL)
            {
            if ((lstPatamar = xmlSuprimento->Get_Patamar()) == NULL) return(false);
            if (!MontaDadosCurvaSuprimentoEqv(suprimento->curva, lstPatamar)) return(false);
            }
         // Salva as medições
         //if (!MontaDadosMedicao(rede, suprimento, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTipoChave(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTipoRede(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTrafo(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   //variáveis locais
   VTTrafo                 *trafo;
   _di_IXMLTEqpto          xmlEqpto;       // Eqpto
   _di_IXMLTId             xmlId;
   _di_IXMLTCnjEqLig       lstEqLig;
   _di_IXMLTEqLigTrafoList  lstTrafo;      // Conjunto de trafoes
   _di_IXMLTEqLigTrafo     xmlTrafo;      // Trafo
   _di_IXMLTCoordId        xmlCoordId;    // Coordenada identificada (Barras)
   _di_IXMLTCoord          xmlCoord;      // Coordenada
   _di_IXMLTEnrolList       lstEnrol;      // Conjunto de enrolamentos
   _di_IXMLTEnrol          xmlEnrol;      // Enrolamento
   _di_IXMLTZseq           xmlZSeq;       // Impedância de seqüência
   _di_IXMLTZ              xmlZ;          // Impedância
   _di_IXMLTLtc            xmlLtc;        // LTC
   VTFases*                fases;

   try{//determina lista de Trafos da Rede
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);

      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRAFO);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstTrafo = lstEqLig->Get_Trafo();

      //loop p/ todos Trafos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trafo = (VTTrafo*)lisEQP->Items[n];

         //insere um novo Trafo
         if ((xmlTrafo = lstTrafo->Add())       == NULL) return(false);
         if ((xmlEqpto = xmlTrafo->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação do Trafo
         if (!MontaDadosIdent(trafo, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Trafo
         if (!MontaDadosCoord(trafo, xmlEqpto->Get_coordId())) return(false);

         //insere atributos do Trafo
         xmlTrafo->Set_intTpoLig(trafo->TipoLigacao());
         xmlTrafo->Set_bolAuto(trafo->auto_trafo);
         xmlTrafo->Set_dblPerFer(trafo->perda_ferro);
         xmlTrafo->Set_dblPotNom(trafo->snom);

         // Enrolamentos
         if ((lstEnrol = xmlTrafo->Get_Enrol()) == NULL) return(false);
         // Primário
         if ((xmlEnrol = lstEnrol->Add()) == NULL) return(false);
         xmlEnrol->Set_intId(1);
         xmlEnrol->Set_intTpoLig(trafo->pri.ligacao);
         xmlEnrol->Set_dblPotNom(trafo->snom);
         xmlEnrol->Set_dblTnsNom(trafo->pri.vnom);
         xmlEnrol->Set_dblTnsTap(trafo->pri.tap);
         xmlEnrol->Set_dblAngDef(trafo->pri.defasagem);
         xmlEnrol->Set_strFases(fases->AsStrABCN[trafo->Fases(trafo->BarraPrimario())]);
         if ((xmlZ = xmlEnrol->Get_zTerra()) == NULL) return(false);
         xmlZ->Set_dblR(trafo->pri.zterra.r);
         xmlZ->Set_dblX(trafo->pri.zterra.x);

         // Secundário
         if ((xmlEnrol = lstEnrol->Add()) == NULL) return(false);
         xmlEnrol->Set_intId(2);
         xmlEnrol->Set_intTpoLig(trafo->sec.ligacao);
         xmlEnrol->Set_dblPotNom(trafo->snom);
         xmlEnrol->Set_dblTnsNom(trafo->sec.vnom);
         xmlEnrol->Set_dblTnsTap(trafo->sec.tap);
         xmlEnrol->Set_dblAngDef(trafo->sec.defasagem);
         xmlEnrol->Set_strFases(fases->AsStrABCN[trafo->Fases(trafo->BarraSecundario())]);
         if ((xmlZ = xmlEnrol->Get_zTerra()) == NULL) return(false);
         xmlZ->Set_dblR(trafo->sec.zterra.r);
         xmlZ->Set_dblX(trafo->sec.zterra.x);

         // LTC - Load Tap Changer
         if ((xmlLtc = xmlTrafo->Get_ltc()) == NULL) return(false);
			xmlLtc->Set_intTpoUtl(trafo->ltc.tipo);
			xmlLtc->Set_dblTns(trafo->ltc.ajuste_auto.v_pu);
			if (trafo->ltc.ajuste_auto.bar_ref)
             {
             if ((xmlCoordId = xmlLtc->Get_coordBarCtl()) == NULL) return(false);
				 xmlCoordId->Set_strCod(IntToStr(trafo->ltc.ajuste_auto.bar_ref->Id));
             if ((xmlCoord = xmlCoordId->Get_coordUTM()) == NULL) return(false);
				 xmlCoord->Set_dblX(trafo->ltc.ajuste_auto.bar_ref->utm.x);
				 xmlCoord->Set_dblY(trafo->ltc.ajuste_auto.bar_ref->utm.y);
             if ((xmlCoord = xmlCoordId->Get_coordESQ1()) == NULL) return(false);
				 xmlCoord->Set_dblX(trafo->ltc.ajuste_auto.bar_ref->esq.x1);
				 xmlCoord->Set_dblY(trafo->ltc.ajuste_auto.bar_ref->esq.y1);
             if ((xmlCoord = xmlCoordId->Get_coordESQ2()) == NULL) return(false);
				 xmlCoord->Set_dblX(trafo->ltc.ajuste_auto.bar_ref->esq.x2);
             xmlCoord->Set_dblY(trafo->ltc.ajuste_auto.bar_ref->esq.y2);
             }

         // Impedâncias
         if ((xmlZSeq = xmlTrafo->Get_zSeq()) == NULL) return(false);

         if ((xmlZ = xmlZSeq->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(trafo->z0.r);
         xmlZ->Set_dblX(trafo->z0.x);
         if ((xmlZ = xmlZSeq->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(trafo->z1.r);
         xmlZ->Set_dblX(trafo->z1.x);

         // Salva as medições
         if (!MontaDadosMedicao(rede, trafo, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTrafo3E(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTTrafo3E                 *trafo3E;
   _di_IXMLTEqpto            xmlEqpto;       // Eqpto
   _di_IXMLTId               xmlId;
   _di_IXMLTCnjEqLig         lstEqLig;
   _di_IXMLTEqLigTrafo3EList  lstTrafo3E;    // Conjunto de trafo3Ees
   _di_IXMLTEqLigTrafo3E     xmlTrafo3E;    // Trafo3E
   _di_IXMLTCoordId          xmlCoordId;    // Coordenada identificada (Barras)
   _di_IXMLTCoord            xmlCoord;      // Coordenada
   _di_IXMLTEnrolList         lstEnrol;      // Conjunto de enrolamentos
   _di_IXMLTEnrol            xmlEnrol;      // Enrolamento
   _di_IXMLTZseq             xmlZSeq;       // Impedância de seqüência
   _di_IXMLTZ                xmlZ;          // Impedância
   VTFases*                  fases;

   try{//determina objeto Fases
      if ((fases = (VTFases*)apl->GetObject(__classid(VTFases))) == NULL) return(false);
      //determina lista de Trafo3Es da Rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRAFO3E);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstTrafo3E = lstEqLig->Get_Trafo3E();

      //loop p/ todos Trafo3Es
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trafo3E = (VTTrafo3E*)lisEQP->Items[n];

         //insere um novo Trafo3E
         if ((xmlTrafo3E = lstTrafo3E->Add())       == NULL) return(false);
         if ((xmlEqpto   = xmlTrafo3E->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação do Trafo3E
         if (!MontaDadosIdent(trafo3E, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Trafo3E
         if (!MontaDadosCoord(trafo3E, xmlEqpto->Get_coordId())) return(false);

         //insere atributos do Trafo 3E
         xmlTrafo3E->Set_intTpoLig(trafo3E->Ligacao);
         xmlTrafo3E->Set_dblPotBas(trafo3E->Sbase);
         xmlTrafo3E->Set_dblPerFer(trafo3E->perda_ferro);

         // Enrolamentos
         if ((lstEnrol = xmlTrafo3E->Get_Enrol()) == NULL) return(false);
         // Primário
         if ((xmlEnrol = lstEnrol->Add()) == NULL) return(false);
         xmlEnrol->Set_intId(1);
         xmlEnrol->Set_intTpoLig(trafo3E->pri.ligacao);
         xmlEnrol->Set_dblPotNom(trafo3E->pri.snom);
         xmlEnrol->Set_dblTnsNom(trafo3E->pri.vnom);
         xmlEnrol->Set_dblTnsTap(trafo3E->pri.tap);
         xmlEnrol->Set_dblAngDef(trafo3E->pri.defasagem);
         xmlEnrol->Set_strFases(fases->AsStrABCN[trafo3E->Fases(trafo3E->pbarra1)]);
         if ((xmlZ = xmlEnrol->Get_zTerra()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->pri.zterra.r);
         xmlZ->Set_dblX(trafo3E->pri.zterra.x);
         // Secundário
         if ((xmlEnrol = lstEnrol->Add()) == NULL) return(false);
         xmlEnrol->Set_intId(2);
         xmlEnrol->Set_intTpoLig(trafo3E->sec.ligacao);
         xmlEnrol->Set_dblPotNom(trafo3E->sec.snom);
         xmlEnrol->Set_dblTnsNom(trafo3E->sec.vnom);
         xmlEnrol->Set_dblTnsTap(trafo3E->sec.tap);
         xmlEnrol->Set_dblAngDef(trafo3E->sec.defasagem);
         xmlEnrol->Set_strFases(fases->AsStrABCN[trafo3E->Fases(trafo3E->pbarra2)]);
         if ((xmlZ = xmlEnrol->Get_zTerra()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->sec.zterra.r);
         xmlZ->Set_dblX(trafo3E->sec.zterra.x);
         // Terciário
         if ((xmlEnrol = lstEnrol->Add()) == NULL) return(false);
         xmlEnrol->Set_intId(3);
         xmlEnrol->Set_intTpoLig(trafo3E->ter.ligacao);
         xmlEnrol->Set_dblPotNom(trafo3E->ter.snom);
         xmlEnrol->Set_dblTnsNom(trafo3E->ter.vnom);
         xmlEnrol->Set_dblTnsTap(trafo3E->ter.tap);
         xmlEnrol->Set_dblAngDef(trafo3E->ter.defasagem);
         xmlEnrol->Set_strFases(fases->AsStrABCN[trafo3E->Fases(trafo3E->pbarra3)]);
         if ((xmlZ = xmlEnrol->Get_zTerra()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->pri.zterra.r);
         xmlZ->Set_dblX(trafo3E->pri.zterra.x);

         // Impedâncias
         if ((xmlZSeq = xmlTrafo3E->Get_zSeqPS()) == NULL) return(false);
         if ((xmlZ = xmlZSeq->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zps0.r);
         xmlZ->Set_dblX(trafo3E->zps0.x);
         if ((xmlZ = xmlZSeq->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zps1.r);
         xmlZ->Set_dblX(trafo3E->zps1.x);

         if ((xmlZSeq = xmlTrafo3E->Get_zSeqPT()) == NULL) return(false);
         if ((xmlZ = xmlZSeq->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zpt0.r);
         xmlZ->Set_dblX(trafo3E->zpt0.x);
         if ((xmlZ = xmlZSeq->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zpt1.r);
         xmlZ->Set_dblX(trafo3E->zpt1.x);

         if ((xmlZSeq = xmlTrafo3E->Get_zSeqST()) == NULL) return(false);
         if ((xmlZ = xmlZSeq->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zst0.r);
         xmlZ->Set_dblX(trafo3E->zst0.x);
         if ((xmlZ = xmlZSeq->Get_z1()) == NULL) return(false);
         xmlZ->Set_dblR(trafo3E->zst1.r);
         xmlZ->Set_dblX(trafo3E->zst1.x);

         // Salva as medições
         if (!MontaDadosMedicao(rede, trafo3E, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTrafoZZ(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTTrafoZZ                    *trafoZZ;
   _di_IXMLTEqpto               xmlEqpto;       // Eqpto
   _di_IXMLTCnjEqBar            lstEqBar;       // Lista de EqBar
   _di_IXMLTEqBarTrafoZZ        xmlTrafoZZ;     // TrafoZZ
   _di_IXMLTEqBarTrafoZZList     lstTrafoZZ;     // Lista de TrafoZZ
   _di_IXMLTZ                   xmlZ;           // Impedância

   try{//determina lista de TrafoZZ da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP, eqptoTRAFOZZ);
      if (lisEQP->Count == 0) return(true);
      //obtém conjunto de EqBar do conjunto Rede
      if ((lstEqBar = xmlRede->Get_cnjEqBar()) == NULL) return(false);
      //obtém conjunto de TrafoZZes
      if ((lstTrafoZZ = lstEqBar->Get_TrafoZZ()) == NULL) return(false);

      //loop p/ todos TrafoZZ da Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trafoZZ = (VTTrafoZZ*)lisEQP->Items[n];

         //insere um novo TrafoZZ
         if ((xmlTrafoZZ = lstTrafoZZ->Add())       == NULL) return(false);
         if ((xmlEqpto      = xmlTrafoZZ->Get_eqbar()) == NULL) return(false);

         //insere atributos de identificação do TrafoZZ
         if (!MontaDadosIdent(trafoZZ, xmlEqpto)) return(false);
         //insere atributos de coordenadas do TrafoZZ
         if (!MontaDadosCoord(trafoZZ, xmlEqpto->Get_coordId()->Add())) return(false);

         //insere atributos do TrafoZZ
         xmlTrafoZZ->Set_dblTnsNom(trafoZZ->vnom);
         xmlTrafoZZ->Set_dblPotNom(trafoZZ->snom);

         if ((xmlZ = xmlTrafoZZ->Get_z0()) == NULL) return(false);
         xmlZ->Set_dblR(trafoZZ->z0.r);
         xmlZ->Set_dblX(trafoZZ->z0.x);

         // Salva as medições
         if (!MontaDadosMedicao(rede, trafoZZ, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaConjuntoTrecho(VTRede *rede, _di_IXMLTRede xmlRede)
   {
   VTTrecho                 *trecho;
   VTArranjo                *arranjo;
   VTSuporte                *suporte;
   VTCabo                   *cabo;
   _di_IXMLTEqpto           xmlEqpto;     // Eqpto
   _di_IXMLTId              xmlId;
   _di_IXMLTCnjEqLig        lstEqLig;
   _di_IXMLTEqLigTrechoList  lstTrecho;    // Conjunto de trechoes
   _di_IXMLTEqLigTrecho     xmlTrecho;    // Trecho
   _di_IXMLTCoordId         xmlCoordId;   // Coordenada identificada (Barras)
   _di_IXMLTCoord           xmlCoord;     // Coordenada
   _di_IXMLTZseq            xmlZSeq;      // Impedância de seqüência
   _di_IXMLTZ               xmlZ;         // Impedância
   _di_IXMLTCSeq            xmlCSeq;      // Capacitância de seqüência
   _di_IXMLTTrcTpo          xmlTrcTpo;    // Tipo de trecho
   _di_IXMLTTrcTpo01        xmlTrcTpo01;  // Trecho tipo 01
   _di_IXMLTTrcTpo02        xmlTrcTpo02;  // Trecho tipo 02
   _di_IXMLTTrcTpo03        xmlTrcTpo03;  // Trecho tipo 03
   _di_IXMLTCaboId          xmlCaboId;    // Identificador de um cabo
   _di_IXMLTCaboId          vecCaboId[4]; // Vetor de cabos ordenados como A, B, C e N

   try{//determina lista de Trechos da Rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRECHO);
      if (lisEQP->Count == 0) return(true);

      //obtém conjunto de Ligacoes da Rede
      lstEqLig = xmlRede->Get_cnjEqLig();
      lstTrecho = lstEqLig->Get_Trecho();

      //loop p/ todos Trechos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trecho = (VTTrecho*)lisEQP->Items[n];

         //insere um novo Trecho
         if ((xmlTrecho = lstTrecho->Add())       == NULL) return(false);
         if ((xmlEqpto  = xmlTrecho->Get_eqlig()) == NULL) return(false);

         //insere atributos de identificação do Trecho
         if (!MontaDadosIdent(trecho, xmlEqpto)) return(false);
         //insere atributos de coordenadas do Trecho
         if (!MontaDadosCoord(trecho, xmlEqpto->Get_coordId())) return(false);

         //insere atributos do Trecho
         xmlTrecho->Set_dblCmp(trecho->Comprimento_km);
         if ((xmlTrcTpo = xmlTrecho->Get_trcTpo()) == NULL) return(false);
         //insere atributos do Arranjo
         if ((arranjo = trecho->arranjo) == NULL) return(false);
         //código do Arranjo
         xmlTrcTpo->Set_strCodArr(arranjo->Codigo);
         //verifica o tipo do Arranjo
         switch (arranjo->TipoCfg)
             {
             case arranjoCABO_Z0Z1: // Tipo 01
                if ((xmlTrcTpo01 = xmlTrcTpo->Get_trcTpo01()) == NULL) return(false);
                if ((xmlCaboId = xmlTrcTpo01->Get_caboId()) == NULL) return(false);

                if (!arranjo->caboZ0Z1) return(false);
                xmlCaboId->SetText(arranjo->caboZ0Z1->Codigo);
                break;

             case arranjoCABOxFASE: // Tipo 02
                if ((xmlTrcTpo02 = xmlTrcTpo->Get_trcTpo02()) == NULL) return(false);
                if ((suporte = arranjo->suporte) == NULL) return(false);

                xmlTrcTpo02->Set_strEstGeoId(suporte->Codigo);
                vecCaboId[0] = xmlTrcTpo02->Get_caboIdA();
                vecCaboId[1] = xmlTrcTpo02->Get_caboIdB();
                vecCaboId[2] = xmlTrcTpo02->Get_caboIdC();
                vecCaboId[3] = xmlTrcTpo02->Get_caboIdN();
                for (int i = 0; i < MAX_FASE; i++)
                   {
                   if ((cabo = arranjo->fase[i].cabo) != NULL)
                      {
                      vecCaboId[i]->SetText(cabo->Codigo);
                      }
                   else
                      {
                      vecCaboId[i]->SetText("");
                      }
                   }
                break;

             case arranjoPU_SB100: // Tipo 03
             case arranjoZ0Z1 :    // Tipo 03  (Somente esse na importação)
             case arranjoMAT_Z:    // Tipo 03
                if ((xmlTrcTpo03 = xmlTrcTpo->Get_trcTpo03()) == NULL) return(false);
                
                xmlZSeq = xmlTrcTpo03->Get_zSeq();
                xmlZ = xmlZSeq->Get_z0();
                xmlZ->Set_dblR(arranjo->z0.r);
                xmlZ->Set_dblX(arranjo->z0.x);
                xmlZ = xmlZSeq->Get_z1();
                xmlZ->Set_dblR(arranjo->z1.r);
                xmlZ->Set_dblX(arranjo->z1.x);
                xmlCSeq = xmlTrcTpo03->Get_cSeq();
                xmlCSeq->Set_dblC0(arranjo->c0);
                xmlCSeq->Set_dblC1(arranjo->c1);
                xmlTrcTpo03->Set_dblCorMax(arranjo->Iadm_amp);
                break;

             default: // Error
                 break;
             }

         // Salva as medições
         if (!MontaDadosMedicao(rede, trecho, xmlEqpto)) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCoord(VTEqpto *eqpto, _di_IXMLTCoordId xmlCoordId, double dblPosBar)
   {
   VTBarra           *barra;
   _di_IXMLTId       xmlId;
   _di_IXMLTCoord    xmlCoordUTM;   // Coordenada UTM
   _di_IXMLTCoord    xmlCoordESQ;   // Coordenada Esquema
   _di_IXMLTCoordIco xmlCoordIco;   // Deslocamentos para posicionamento dos ícones

   // Verifica parâmetros
   if (eqpto == NULL) return(false);
   if (xmlCoordId == NULL) return(false);

   // unck@20080514 - OOPS: em algumas siuações, para o gerador está retornando TipoLigacao() true!!!
   // if (eqpto->TipoLigacao()) return(false);

   if (eqpto->TipoEqbar())
      {
      barra = ((VTEqbar*)eqpto)->pbarra;

      // unck@20080806 - coloca o valor da posição da barra.
      //                 EqBar ignora o posbar passado como parâmetro.
      xmlCoordId->Set_dblPosBar(((VTEqbar*)eqpto)->esq.posbar);

      // unck@20080807 - Valores setados somente para EqBar
      xmlCoordIco = xmlCoordId->Get_coordIco();
      xmlCoordIco->Set_dblDxBar(((VTEqbar*)eqpto)->esq.dxbar);
      xmlCoordIco->Set_dblDyBar(((VTEqbar*)eqpto)->esq.dybar);
      }
   else
      {
      barra = (VTBarra*)eqpto;

      // unck@20080807 - coloca o valor da posição da barra
      xmlCoordId->Set_dblPosBar(dblPosBar);
      }
   if (barra == NULL) return(false);
   /*
   // Copia valores
   xmlCoordId->Set_strCod(barra->Codigo);
   xmlCoordUTM = xmlCoordId->Get_coordUTM();
   xmlCoordUTM->Set_dblX(barra->utm.x);
   xmlCoordUTM->Set_dblY(barra->utm.y);
   xmlCoordESQ = xmlCoordId->Get_coordESQ1();
   xmlCoordESQ->Set_dblX(barra->esq.x1);
   xmlCoordESQ->Set_dblY(barra->esq.y1);
   xmlCoordESQ = xmlCoordId->Get_coordESQ2();
   xmlCoordESQ->Set_dblX(barra->esq.x2);
   xmlCoordESQ->Set_dblY(barra->esq.y2);
   */
   //preenche xmlId c/ Id e Código da  Barra
   if ((xmlId   = xmlCoordId->Get_id()) != NULL)
      {
      xmlId->Set_intId(barra->Id);
      xmlId->Set_strCod(barra->Codigo);
      }
    // Copia valores
   //xmlCoordId->Set_intCoordNdx(barra->index);
   xmlCoordId->Set_intCoordNdx(0);
   xmlCoordId->Set_strCod(barra->Codigo);
   xmlCoordUTM = xmlCoordId->Get_coordUTM();
   xmlCoordUTM->Set_dblX(double(barra->utm.x)/100.);
   xmlCoordUTM->Set_dblY(double(barra->utm.y)/100.);
   xmlCoordESQ = xmlCoordId->Get_coordESQ1();
   xmlCoordESQ->Set_dblX(double(barra->esq.x1)/100.);
   xmlCoordESQ->Set_dblY(double(barra->esq.y1)/100.);
   xmlCoordESQ = xmlCoordId->Get_coordESQ2();
   xmlCoordESQ->Set_dblX(double(barra->esq.x2)/100.);
   xmlCoordESQ->Set_dblY(double(barra->esq.y2)/100.);

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCoord(VTEqpto *eqpto, _di_IXMLTCoordIdList lstCoord)
   {
   VTBarra             *barra;
   VTLigacao           *ligacao;
   _di_IXMLTCoordId    xmlCoord;
   double              vecposBar[NBAR_LIG];  // unck@20080807

   // Verifica parâmetros
   if (eqpto == NULL) return(false);
   if (lstCoord == NULL) return(false);
   if (!eqpto->TipoLigacao()) return(false);
   if ((ligacao = (VTLigacao*)eqpto) == NULL) return(false);

   // unck@20080807
   // Solução toscana: para cada barra, armazena o posicionamento
   vecposBar[0] = ((VTLigacao*)eqpto)->esq.posbar1;
   vecposBar[1] = ((VTLigacao*)eqpto)->esq.posbar2;
   vecposBar[2] = ((VTLigacao*)eqpto)->esq.posbar3;

   // Executa rotina para armazenamento de valores
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if ((xmlCoord = lstCoord->Add()) == NULL) return(false);
      if ((MontaDadosCoord(barra, xmlCoord, vecposBar[ind_bar])) == false) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCurva(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList)
   {
   _di_IXMLTPatamProp  xmlPatamar;     // Patamar da curva
   strHM               hm_ini, hm_fim;
   union               {
                       double vet[4];
                       struct {double p, q, v, t;} var;
                       } valor;

   if (!xmlPatamarList) return(false);

   for (int i = 0; i < curva->NumeroPontos(); i++)
       {
       if (curva->GetPonto(i, hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double)) == true)
           {
           // leu o ponto, então registra o patamar
           if ((xmlPatamar = xmlPatamarList->Add()) == NULL) return(false);
           xmlPatamar->horIni->Set_intHor(hm_ini.hora);
           xmlPatamar->horIni->Set_intMin(hm_ini.minuto);
           xmlPatamar->horFim->Set_intHor(hm_fim.hora);
           xmlPatamar->horFim->Set_intMin(hm_fim.minuto);
           xmlPatamar->Set_dblPotAtv(valor.var.p * 1000.); //MW   => kW
           xmlPatamar->Set_dblPotRtv(valor.var.q * 1000.); //MVAr => kVAr
           xmlPatamar->Set_dblMdlTns(valor.var.v);
           xmlPatamar->Set_dblAng(valor.var.t);
           }
       }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCurvaCargaEqv(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList)
   {
   _di_IXMLTPatamProp          xmlPatamar;     // Patamar da curva
   strHM                       hm_ini, hm_fim;
   double                      vet[nvCURVA_RRCAR];
   struct {double p, q, v, t;} var;

   if (!xmlPatamarList) return(false);

   for (int i = 0; i < curva->NumeroPontos(); i++)
       {
       if (curva->GetPonto(i, hm_ini, hm_fim, vet, sizeof(vet)/sizeof(double)))
           {//acumula valores de demanda das 4 fases
           var.p = vet[0] + vet[2] + vet[4] + vet[6];
           var.q = vet[1] + vet[3] + vet[5] + vet[7];
           var.v = 0;
           var.t = 0;
           //insere o patamar
           if ((xmlPatamar = xmlPatamarList->Add()) == NULL) return(false);
           xmlPatamar->horIni->Set_intHor(hm_ini.hora);
           xmlPatamar->horIni->Set_intMin(hm_ini.minuto);
           xmlPatamar->horFim->Set_intHor(hm_fim.hora);
           xmlPatamar->horFim->Set_intMin(hm_fim.minuto);
           xmlPatamar->Set_dblPotAtv(var.p * 1000.); //MW   => kW
           xmlPatamar->Set_dblPotRtv(var.q * 1000.); //MVAr => kVAr
           xmlPatamar->Set_dblMdlTns(var.v);
           xmlPatamar->Set_dblAng(var.t);
           }
       }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCurvaSuprimentoEqv(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList)
   {
   _di_IXMLTPatamProp          xmlPatamar;     // Patamar da curva
   strHM                       hm_ini, hm_fim;
   double                      vet[MAX_FASE * nvCURVA_PQVT];
   struct {double p, q, v, t;} var;

   if (!xmlPatamarList) return(false);

   for (int i = 0; i < curva->NumeroPontos(); i++)
       {
       if (curva->GetPonto(i, hm_ini, hm_fim, vet, sizeof(vet)/sizeof(double)))
           {//acumula valores de demanda das 4 fases
           var.p = 0;
           var.q = 0;
           var.v = vet[2];
           var.t = vet[3];
           //insere o patamar
           if ((xmlPatamar = xmlPatamarList->Add()) == NULL) return(false);
           xmlPatamar->horIni->Set_intHor(hm_ini.hora);
           xmlPatamar->horIni->Set_intMin(hm_ini.minuto);
           xmlPatamar->horFim->Set_intHor(hm_fim.hora);
           xmlPatamar->horFim->Set_intMin(hm_fim.minuto);
           xmlPatamar->Set_dblPotAtv(var.p * 1000.); //MW   => kW
           xmlPatamar->Set_dblPotRtv(var.q * 1000.); //MVAr => kVAr
           xmlPatamar->Set_dblMdlTns(var.v);
           xmlPatamar->Set_dblAng(var.t);
           }
       }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosCurvaTip(VTCurva *curva, _di_IXMLTCurvaTipicaList xmlCurvaTipicaList, bool bolCurvaCarga)
   {
   //variáveis locais
   double               valor[32];
   strHM                hm_ini, hm_fim;
   int                  num_pto   = curva->NumeroPontos();
   int                  dim_valor = curva->NumeroValores;
   _di_IXMLTCurvaTipica xmlCurvaTipica = NULL;
   _di_IXMLTId          xmlId          = NULL;
   _di_IXMLTPatamTipList xmlPatamTipList = NULL;
   _di_IXMLTPatamTip    xmlPatamTip    = NULL;
   _di_IXMLTHorMin      xmlHorMin      = NULL;
   _di_IXMLdouble_List   xmlValList      = NULL;

   if ((xmlCurvaTipica = xmlCurvaTipicaList->Add()) == NULL)      return(false);
   if ((xmlPatamTipList = xmlCurvaTipica->Get_Patamar()) == NULL) return(false);
   if ((xmlId = xmlCurvaTipica->Get_id()) == NULL)               return(false);

   xmlId->Set_intId(curva->Id);
   xmlId->Set_strCod(curva->Codigo);
   // OOPS: não há atributos para ano e mês, ainda
   xmlCurvaTipica->Set_intAno(0);
   xmlCurvaTipica->Set_intMes(0);
   xmlCurvaTipica->Set_intTpoCurva(curva->Tipo);
   xmlCurvaTipica->Set_intUnidade(curva->Unidade);
   xmlCurvaTipica->Set_intEscala(curva->Escala);
   //classe de consumidor
   if (curva->Classe) xmlCurvaTipica->Set_intTpoCons(curva->Classe->Id);
   else               xmlCurvaTipica->Set_intTpoCons(-1);
   // Parâmetros exclusivos para curvas de carga
   if (bolCurvaCarga)
      {
      xmlCurvaTipica->Set_dblEneMin(curva->LimiteInferior);
      xmlCurvaTipica->Set_dblEneMax(curva->LimiteSuperior);
      }
   // Dados da curva propriamente ditos
   xmlCurvaTipica->Set_intNumPat(num_pto);
   for (int index = 0; index < num_pto; index++)
      {
      curva->GetPonto(index, hm_ini, hm_fim, valor, dim_valor);
      if ((xmlPatamTip = xmlPatamTipList->Add()) == NULL)    return(false);
      if ((xmlHorMin = xmlPatamTip->Get_horIni()) == NULL)  return(false);
      xmlHorMin->Set_intHor(hm_ini.hora);
      xmlHorMin->Set_intMin(hm_ini.minuto);
      if ((xmlHorMin = xmlPatamTip->Get_horFim()) == NULL)  return(false);
      xmlHorMin->Set_intHor(hm_fim.hora);
      xmlHorMin->Set_intMin(hm_fim.minuto);
      xmlPatamTip->Set_intNumVal(dim_valor);
      if ((xmlValList = xmlPatamTip->Get_dblVal()) == NULL)  return(false);
      for (int n = 0; n < dim_valor; n++)
         {
         xmlValList->Add(valor[n]);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosIdent(VTEqpto *eqpto, _di_IXMLTId xmlId)
   {
   if (xmlId == NULL) return(false);

   //insere atributos de identificação da Ligacao
   xmlId->Set_intId(eqpto->Id);
   xmlId->Set_strCod(eqpto->Codigo);
   xmlId->Set_strDsc("");
   xmlId->Set_strExtId(eqpto->Extern_id);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosIdent(VTEqpto *eqpto, _di_IXMLTEqpto xmlEqpto)
   {
   if (xmlEqpto == NULL) return(false);

   return(MontaDadosIdent(eqpto, xmlEqpto->Get_id()));
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaDadosMedicao(VTRede *rede, VTEqpto *eqpto, _di_IXMLTEqpto xmlEqpto)
    {
    TList*              lstMed;
    TList*              lisCANAL;
    VTMedidor*          medidor;
    VTCanal*            canal;
    _di_IXMLTMedicaoList lstMedicao;     // Lista de Medições
    _di_IXMLTMedicao    xmlMedicao;     // Medicao (Medidor e Canal)
    _di_IXMLTId         xmlId;       // Identificador do medidor e canal

    try {//obtém lista de medidores
        lstMed = new TList();
        rede->LisEqbar(lstMed, eqptoMEDIDOR);
        //loop p/ todos medidores
        for (int i = 0; i < lstMed->Count; i++)
            {
            medidor  = (VTMedidor*)lstMed->Items[i];
            lisCANAL = medidor->LisCanal();
            for (int nc = 0; nc < lisCANAL->Count; nc++)
                {
                canal = (VTCanal*)lisCANAL->Items[nc];
                if (canal->Eqpto != eqpto) continue;
                // Exporta os dados do canal (que seria uma medição, no XML)
                lstMedicao = xmlEqpto->Get_Medicao();
                xmlMedicao = lstMedicao->Add();

                xmlId = xmlMedicao->Get_medidorId();
                xmlId->Set_intId(medidor->Id);
                xmlId->Set_strCod(medidor->Codigo);
                xmlId->Set_strDsc("");
                xmlId->Set_strExtId(medidor->Extern_id);

                xmlId = xmlMedicao->Get_canalId();
                xmlId->Set_intId(0);
                xmlId->Set_strCod(canal->Codigo);
                xmlId->Set_strDsc("");
                xmlId->Set_strExtId("");

                xmlMedicao->Set_dblSigmaCnl(canal->Tolerancia_perc);
                xmlMedicao->Set_intTpoCnl(canal->TipoCanal);

                MontaDadosCurva(canal->Curva, xmlMedicao->Get_Patamar());
                }
            }
        delete lstMed;
        }
    catch(Exception &e)
        {
        return(false);
        }

    return(true);
    } // MontaDadosMedicao

//---------------------------------------------------------------------------
bool __fastcall TExportaXML::MontaUnicaRede(VTRede *rede)
   {        
   //variáveis locais
   _di_IXMLTRede    xmlRede;
   _di_IXMLTRedeList xmlRedeList;
   VTDominio        *dominio;
   VTRedes          *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{
      //obtém objeto xmlListRede
      xmlRedeList = xmlConcessAux->Get_Rede();
      //insere um objeto xmlRede em
      xmlRede = xmlRedeList->Add();
      //monta dados da rede
      if (! MontaDadosIdent(rede, xmlRede->Get_id())) return(false);
      //insere TipoRede
      xmlRede->Set_strTpo(rede->TipoRede->Codigo);
      //insere Cluster
      if (rede->Cluster != NULL) xmlRede->Set_strCluster(rede->Cluster->Codigo);
      //insere Dominio
      lisEQP->Clear();
      redes->LisDominio(lisEQP, rede);
      if (lisEQP->Count > 0)
         {
         dominio = (VTDominio*)lisEQP->First();
         xmlRede->Set_strDominio(dominio->Codigo);
         }
      //insere dados dos objetos EqBar conectados às Barras da Rede
      if (! MontaConjuntoMedidor(rede, xmlRede))    return(false);
      if (! MontaConjuntoCapacitor(rede, xmlRede))  return(false);
      if (! MontaConjuntoCarga(rede, xmlRede))      return(false);
      if (! MontaConjuntoCargaIP(rede, xmlRede))    return(false);
      if (! MontaConjuntoFiltro(rede, xmlRede))     return(false);
      if (! MontaConjuntoGerador(rede, xmlRede))    return(false);
      if (! MontaConjuntoReator(rede, xmlRede))     return(false);
      if (! MontaConjuntoSuprimento(rede, xmlRede)) return(false);
      if (! MontaConjuntoTrafoZZ(rede, xmlRede))    return(false);
      //insere dados dos objetos Ligacao da Rede
      if (! MontaConjuntoCapserie(rede, xmlRede))   return(false);
      if (! MontaConjuntoChave(rede, xmlRede))      return(false);
      if (! MontaConjuntoRegulador(rede, xmlRede))  return(false);
      if (! MontaConjuntoTrafo(rede, xmlRede))      return(false);
      if (! MontaConjuntoTrafo3E(rede, xmlRede))    return(false);
      if (! MontaConjuntoTrecho(rede, xmlRede))     return(false);
      //insere as Mútuas
      if (! MontaConjuntoMutua(rede, xmlRede))      return(false);

      }catch(Exception &e)
         {
         return(false);
         }

   return(true);
   }
//-----------------------------------------------------------------------------
// eof


