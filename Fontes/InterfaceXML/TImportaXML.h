//---------------------------------------------------------------------------
#ifndef TImportaXMLH
#define TImportaXMLH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaXML.h"
#include "SinapModeloRede_v52.h"

//---------------------------------------------------------------------------
class VTCurva;
class VTEqpto;
class VTRede;
class VTFases;
class VTLog;
class VTMonta;
class VTFormato;
class VTArranjos;

//---------------------------------------------------------------------------

class TImportaXML : public VTImportaXML
   {
   public:
           __fastcall  TImportaXML(VTApl *apl_owner);
           __fastcall ~TImportaXML(void);
      bool __fastcall  ExecutaAvaliacao(AnsiString arq_xml);
      bool __fastcall  ExecutaImportacao(AnsiString arq_xml);
      bool __fastcall  ImportaArranjos(AnsiString arq_xml);
      bool __fastcall  ImportaConfiguracao(AnsiString arq_txt);
      bool __fastcall  ImportaCurvasTipicas(AnsiString arq_xml);
      bool __fastcall  ImportaRedesApenas(AnsiString arq_xml);
      void __fastcall  RelatorioLog(TStrings *lines);

   private: //métodos
      int        __fastcall CriaBarra(_di_IXMLTCoordId xmlCoordId);
      VTCurva*   __fastcall CriaCurvaTipica(_di_IXMLTCurvaTipica xmlCurva, bool bolCurvaCarga = false);
      AnsiString __fastcall DefineCodigoUnicoArranjo(AnsiString cod_orig);
      VTEqpto*   __fastcall InsereCluster(AnsiString codigo);
      VTEqpto*   __fastcall InsereDominio(AnsiString codigo);
      //bool     __fastcall InsereEstudo(void);
      bool       __fastcall InsereConjuntoArranjo(void);
      bool       __fastcall InsereConjuntoArranjoCaboRGMR(_di_IXMLTArranjoCaboRGMRList lstArranjo);
      bool       __fastcall InsereConjuntoArranjoCaboZ0Z1(_di_IXMLTArranjoCaboZ0Z1List lstArranjo);
      bool       __fastcall InsereConjuntoArranjoZ0Z1(_di_IXMLTArranjoZ0Z1List lstArranjo);
      bool       __fastcall InsereConjuntoCaboMonopolar(_di_IXMLTCaboRGMRList lstCabo);
      bool       __fastcall InsereConjuntoCaboMultiplexado(_di_IXMLTCaboZ0Z1List lstCabo);
      bool       __fastcall InsereConjuntoCapacitor(_di_IXMLTEqBarCapacitorList lstCapacitor, VTRede* rede);
      bool       __fastcall InsereConjuntoCapserie(_di_IXMLTEqLigCapacitorSerieList lstCapacitorSerie, VTRede* rede);
      bool       __fastcall InsereConjuntoCarga(_di_IXMLTEqBarCargaList lstCarga, VTRede* rede);
      bool       __fastcall InsereConjuntoCargaIP(_di_IXMLTEqBarCargaIPList lstCargaIP, VTRede* rede);
      bool       __fastcall InsereConjuntoChave(_di_IXMLTEqLigChaveList lstChave, VTRede* rede);
      bool       __fastcall InsereConjuntoCurvasTipicas(void);
      bool       __fastcall InsereConjuntoCurvasTipicasCarga(_di_IXMLTCurvaTipicaList lstCurva);
      bool       __fastcall InsereConjuntoCurvasTipicasCargaIP(_di_IXMLTCurvaTipicaList lstCurva);
      bool       __fastcall InsereConjuntoCurvasTipicasCapacitor(_di_IXMLTCurvaTipicaList lstCurva);
      bool       __fastcall InsereConjuntoCurvasTipicasGeradorFoto(_di_IXMLTCurvaTipicaList lstCurva);
      bool       __fastcall InsereConjuntoCurvasTipicasReator(_di_IXMLTCurvaTipicaList lstCurva);
      bool       __fastcall InsereConjuntoFiltro(_di_IXMLTEqBarFiltroList lstFiltro, VTRede* rede);
      bool       __fastcall InsereConjuntoGerador(_di_IXMLTEqBarGeradorList lstGerador, VTRede* rede);
      bool       __fastcall InsereConjuntoMedidor(_di_IXMLTEqBarMedidorList lstMedidor, VTRede* rede);
      bool       __fastcall InsereConjuntoMutua(_di_IXMLTMutuaList lstMutua);
      bool       __fastcall InsereConjuntoReator(_di_IXMLTEqBarReatorList lstReator, VTRede* rede);
      bool       __fastcall InsereConjuntoRede(_di_IXMLTRedeList lstRede);
      bool       __fastcall InsereConjuntoRegulador(_di_IXMLTEqLigReguladorList lstRegulador, VTRede* rede);
      bool       __fastcall InsereConjuntoSuporte(_di_IXMLTSuporteList lstSuporte);
      bool       __fastcall InsereConjuntoSuprimento(_di_IXMLTEqBarSuprimentoList lstSuprimento, VTRede* rede);
      bool       __fastcall InsereConjuntoTrafo(_di_IXMLTEqLigTrafoList lstTrafo, VTRede* rede);
      bool       __fastcall InsereConjuntoTrafo3E(_di_IXMLTEqLigTrafo3EList lstTrafo3E, VTRede* rede);
      bool       __fastcall InsereConjuntoTrafoZZ(_di_IXMLTEqBarTrafoZZList lstTrafoZZ, VTRede* rede);
      bool       __fastcall InsereConjuntoTrecho(_di_IXMLTEqLigTrechoList lstTrecho, VTRede* rede);
      VTRede*    __fastcall InsereRede(_di_IXMLTRede xmlRede);
      bool       __fastcall InsereRedes(void);
      bool       __fastcall InsereMedicao(_di_IXMLTEqpto xmlEqpto, VTEqpto* eqpto);
      void       __fastcall MsgLog(AnsiString txt, VTRede* rede=NULL);
      double     __fastcall ValidaComprimento(double x1_m, double y1_m,
                                           double x2_m, double y2_m, double comp_km);

   private: //dados locais
      int        dxbar, dybar;
      VTApl      *apl;
      VTArranjos *arranjos;
      VTFases    *fases;
      VTLog      *plog;
      VTMonta    *monta;
      VTFormato  *formato;
      TList      *lisEQP;
      //opções de importação
      struct{
            bool arranjo;
            bool curvatipica;
            bool tipochave;
            bool tiporede;
            bool rede;
            }opcao;
      // Objetos gerados pelo borland (interface _di_XML*.*)
     _di_IXMLTConcessionaria xmlConcessAux;   // Concessionaria
   };

#endif
//-----------------------------------------------------------------------------
// eof

