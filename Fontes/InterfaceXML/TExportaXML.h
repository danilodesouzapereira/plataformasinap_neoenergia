//---------------------------------------------------------------------------
#ifndef TExportaXMLH
#define TExportaXMLH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaXML.h"
#include "SinapModeloRede_v52.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTEqpto;
class VTRede;
class VTRedes;
class VTReducao;
class VTCurva;

//---------------------------------------------------------------------------

class TExportaXML : public VTExportaXML
   {
   public:
           __fastcall  TExportaXML(VTApl *apl_owner);
           __fastcall ~TExportaXML(void);
      bool __fastcall  ExportaArranjos(AnsiString arq_xml);
      bool __fastcall  ExportaCurvasTipicas(AnsiString arq_xml);
      bool __fastcall  ExportaRedes(AnsiString arq_xml);
      bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_xml);

   private: //métodos
      void __fastcall AjustaCoordenadaBarra(void);
      bool __fastcall Monta(VTRede *rede);
      bool __fastcall MontaConjuntoAllRede(void);
      bool __fastcall MontaConjuntoArranjo(void);
      bool __fastcall MontaConjuntoArranjoCaboRGMR(VTArranjo* arranjo);
      bool __fastcall MontaConjuntoArranjoCaboZ0Z1(VTArranjo* arranjo);
      bool __fastcall MontaConjuntoArranjoZ0Z1(VTArranjo* arranjo);
      bool __fastcall MontaConjuntoCaboMonopolar(void);
      bool __fastcall MontaConjuntoCaboMultiplexado(void);
      bool __fastcall MontaConjuntoCapacitor(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoCapserie(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoCarga(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoCarga(VTReducao *reducao, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoCargaIP(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoChave(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoConcessionaria(void);
      bool __fastcall MontaConjuntoCurvasTipicas(void);
      bool __fastcall MontaConjuntoCurvasTipicasCarga(void);
      bool __fastcall MontaConjuntoCurvasTipicasCargaIP(void);
      bool __fastcall MontaConjuntoCurvasTipicasCapacitor(void);
      bool __fastcall MontaConjuntoCurvasTipicasGeradorFoto(void);
      bool __fastcall MontaConjuntoCurvasTipicasReator(void);
      bool __fastcall MontaConjuntoFiltro(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoGerador(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoMedidor(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoMutua(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoReator(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoRede(VTRede *rede);
      bool __fastcall MontaConjuntoReducao(void);
      bool __fastcall MontaConjuntoRegulador(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoSuporte(void);
      bool __fastcall MontaConjuntoSuprimento(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoSuprimento(VTReducao *reducao, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoTipoChave(void);
      bool __fastcall MontaConjuntoTipoRede(void);
      bool __fastcall MontaConjuntoTrafo(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoTrafo3E(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoTrecho(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaConjuntoTrafoZZ(VTRede *rede, _di_IXMLTRede xmlRede);
      bool __fastcall MontaDadosCoord(VTEqpto *eqpto, _di_IXMLTCoordId xmlCoordId, double dblPosBar = 0);
      bool __fastcall MontaDadosCoord(VTEqpto *eqpto, _di_IXMLTCoordIdList lstCoord);
      bool __fastcall MontaDadosCurva(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList);
      bool __fastcall MontaDadosCurvaCargaEqv(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList);
      bool __fastcall MontaDadosCurvaSuprimentoEqv(VTCurva *curva, _di_IXMLTPatamPropList xmlPatamarList);
      bool __fastcall MontaDadosCurvaTip(VTCurva *curva, _di_IXMLTCurvaTipicaList xmlCurvaTipicaList, bool bolCurvaCarga);
      bool __fastcall MontaDadosIdent(VTEqpto *eqpto, _di_IXMLTId xmlId);
      bool __fastcall MontaDadosIdent(VTEqpto *eqpto, _di_IXMLTEqpto xmlEqpto);
      bool __fastcall MontaDadosMedicao(VTRede *rede, VTEqpto *eqpto, _di_IXMLTEqpto xmlEqpto);
      bool __fastcall MontaUnicaRede(VTRede *rede);


   private: //objetos externos

   private: //dados locais
      VTApl       *apl;
      TList       *lisEQP;

      // Objetos gerados pelo borland (interface _di_XML*.*)
     _di_IXMLTConcessionaria xmlConcessAux;   // Concessionaria
     AnsiString              strEncoding;     // Codificação do arquivo

   };

#endif
//-----------------------------------------------------------------------------
// eof


