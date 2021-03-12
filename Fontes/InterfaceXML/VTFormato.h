//-----------------------------------------------------------------------------
#ifndef VTFormatoH
#define VTFormatoH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "SinapModeloRede_v52.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTFormato : public TObject
   {
   public:
                   __fastcall  VTFormato(void) {};
      virtual      __fastcall ~VTFormato(void) {};
		virtual bool __fastcall  Executa(_di_IXMLTConcessionaria xmlConcessAux)=0;
		//validação de conjuntos de objetos
		virtual bool __fastcall ValidaConjuntoCaboMonopolar(_di_IXMLTCaboRGMRList XMLCaboRGMRList) = 0;
		virtual bool __fastcall ValidaConjuntoCaboMultiplexado(_di_IXMLTCaboZ0Z1List XMLCaboZ0Z1List) = 0;
		virtual bool __fastcall ValidaConjuntoCapacitor(_di_IXMLTEqBarCapacitorList ListCapacitor) = 0;
		virtual bool __fastcall ValidaConjuntoCapserie(_di_IXMLTEqLigCapacitorSerieList ListCapacitorSerie) = 0;
		virtual bool __fastcall ValidaConjuntoCarga(_di_IXMLTEqBarCargaList ListCarga) = 0;
		virtual bool __fastcall ValidaConjuntoCargaIP(_di_IXMLTEqBarCargaIPList ListCargaIP) = 0;
		virtual bool __fastcall ValidaConjuntoChave(_di_IXMLTEqLigChaveList ListChave) = 0;
		virtual bool __fastcall ValidaConjuntoFiltro(_di_IXMLTEqBarFiltroList ListFiltro) = 0;
		virtual bool __fastcall ValidaConjuntoGerador(_di_IXMLTEqBarGeradorList ListGerador) = 0;
		virtual bool __fastcall ValidaConjuntoMedicao(_di_IXMLTMedicaoList cnjMedicao) = 0;
		virtual bool __fastcall ValidaConjuntoMedidor(_di_IXMLTEqBarMedidorList ListMedidor) = 0;
		virtual bool __fastcall ValidaConjuntoMutua(_di_IXMLTMutuaList ListMutua) = 0;
		virtual bool __fastcall ValidaConjuntoReator(_di_IXMLTEqBarReatorList ListReator) = 0;
		virtual bool __fastcall ValidaConjuntoRede(_di_IXMLTRedeList XMLRedeList) = 0;
		virtual bool __fastcall ValidaConjuntoRegulador(_di_IXMLTEqLigReguladorList ListRegulador) = 0;
		virtual bool __fastcall ValidaConjuntoSuporte(_di_IXMLTSuporteList XMLSuporteList) = 0;
		virtual bool __fastcall ValidaConjuntoSuprimento(_di_IXMLTEqBarSuprimentoList ListSuprimento) = 0;
		virtual bool __fastcall ValidaConjuntoTrafo(_di_IXMLTEqLigTrafoList ListTrafo) = 0;
		virtual bool __fastcall ValidaConjuntoTrafo3E(_di_IXMLTEqLigTrafo3EList ListTrafo3E) = 0;
		virtual bool __fastcall ValidaConjuntoTrafoZZ(_di_IXMLTEqBarTrafoZZList ListTrafoZZ) = 0;
		virtual bool __fastcall ValidaConjuntoTrecho(_di_IXMLTEqLigTrechoList ListTrecho) = 0;
		//validação de objetos
		virtual bool __fastcall ValidaObjetoCaboRGMR(_di_IXMLTCaboRGMR XMLCaboRGMR) = 0;
      virtual bool __fastcall ValidaObjetoCaboZ0Z1(_di_IXMLTCaboZ0Z1 XMLCaboZ0Z1) = 0;
		virtual bool __fastcall ValidaObjetoConcessionaria(_di_IXMLTConcessionaria xmlConcessAux) = 0;
		virtual bool __fastcall ValidaObjetoCoordId(_di_IXMLTCoordId XMLCoordId) = 0;
      virtual bool __fastcall ValidaObjetoCSeq(_di_IXMLTCSeq xmlCSeq) = 0;
		virtual bool __fastcall ValidaObjetoEnrol(_di_IXMLTEnrol xmlEnrol) = 0;
      virtual bool __fastcall ValidaObjetoEqpto(_di_IXMLTEqpto xmlEqpto, int num_coordId) = 0;
		virtual bool __fastcall ValidaObjetoID(_di_IXMLTId XMLId) = 0;
		virtual bool __fastcall ValidaObjetoLtc( _di_IXMLTLtc xmlLtc) = 0;
		virtual bool __fastcall ValidaObjetoMedicao(_di_IXMLTMedicao xmlMedicao) = 0;
		virtual bool __fastcall ValidaObjetoPatamar(_di_IXMLTPatamProp xmlPatamProp) = 0;
      virtual bool __fastcall ValidaObjetoPotCpx(_di_IXMLTPotCpx xmlPotCpx) = 0;
		virtual bool __fastcall ValidaObjetoRede(_di_IXMLTRede XMLRede) = 0;
      virtual bool __fastcall ValidaObjetoZ(_di_IXMLTZ xmlZ) = 0;
		virtual bool __fastcall ValidaObjetoZSeq(_di_IXMLTZseq xmlZSeq) = 0;
	};

//-----------------------------------------------------------------------------
// função para criar objeto VTFormato
//-----------------------------------------------------------------------------
VTFormato* __fastcall NewObjFormato(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
