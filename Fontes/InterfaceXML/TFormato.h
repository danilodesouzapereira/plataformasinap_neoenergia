//-----------------------------------------------------------------------------
#ifndef TFormatoH
#define TFormatoH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFormato.h"

//---------------------------------------------------------------------------
class VTLog;

//---------------------------------------------------------------------------
class TFormato : public VTFormato
   {
   public:
            __fastcall  TFormato(VTApl *apl);
            __fastcall ~TFormato(void);
      bool  __fastcall  Executa(_di_IXMLTConcessionaria xmlConcessAux);
		//validação de conjuntos de objetos
		bool __fastcall ValidaConjuntoCaboMonopolar(_di_IXMLTCaboRGMRList XMLCaboRGMRList);
		bool __fastcall ValidaConjuntoCaboMultiplexado(_di_IXMLTCaboZ0Z1List XMLCaboZ0Z1List);
		bool __fastcall ValidaConjuntoCapacitor(_di_IXMLTEqBarCapacitorList lstCapacitor);
		bool __fastcall ValidaConjuntoCapserie(_di_IXMLTEqLigCapacitorSerieList lstCapacitorSerie);
		bool __fastcall ValidaConjuntoCarga(_di_IXMLTEqBarCargaList lstCarga);
		bool __fastcall ValidaConjuntoCargaIP(_di_IXMLTEqBarCargaIPList lstCargaIP);
		bool __fastcall ValidaConjuntoChave(_di_IXMLTEqLigChaveList lstChave);
		bool __fastcall ValidaConjuntoFiltro(_di_IXMLTEqBarFiltroList lstFiltro);
		bool __fastcall ValidaConjuntoGerador(_di_IXMLTEqBarGeradorList lstGerador);
		bool __fastcall ValidaConjuntoMedicao(_di_IXMLTMedicaoList cnjMedicao);
		bool __fastcall ValidaConjuntoMedidor(_di_IXMLTEqBarMedidorList lstMedidor);
		bool __fastcall ValidaConjuntoMutua(_di_IXMLTMutuaList lstMutua);
		bool __fastcall ValidaConjuntoReator(_di_IXMLTEqBarReatorList lstReator);
		bool __fastcall ValidaConjuntoRede(_di_IXMLTRedeList XMLRedeList);
		bool __fastcall ValidaConjuntoRegulador(_di_IXMLTEqLigReguladorList lstRegulador);
		bool __fastcall ValidaConjuntoSuporte(_di_IXMLTSuporteList XMLSuporteList);
		bool __fastcall ValidaConjuntoSuprimento(_di_IXMLTEqBarSuprimentoList lstSuprimento);
		bool __fastcall ValidaConjuntoTrafo(_di_IXMLTEqLigTrafoList lstTrafo);
		bool __fastcall ValidaConjuntoTrafo3E(_di_IXMLTEqLigTrafo3EList lstTrafo3E);
		bool __fastcall ValidaConjuntoTrafoZZ(_di_IXMLTEqBarTrafoZZList lstTrafoZZ);
		bool __fastcall ValidaConjuntoTrecho(_di_IXMLTEqLigTrechoList lstTrecho);
		//validação de objetos
		bool __fastcall ValidaObjetoCaboRGMR(_di_IXMLTCaboRGMR XMLCaboRGMR);
      bool __fastcall ValidaObjetoCaboZ0Z1(_di_IXMLTCaboZ0Z1 XMLCaboZ0Z1);
		bool __fastcall ValidaObjetoConcessionaria(_di_IXMLTConcessionaria xmlConcessAux);
      bool __fastcall ValidaObjetoCoordId(_di_IXMLTCoordId XMLCoordId);
      bool __fastcall ValidaObjetoCSeq(_di_IXMLTCSeq xmlCSeq);
		bool __fastcall ValidaObjetoEnrol(_di_IXMLTEnrol xmlEnrol);
      bool __fastcall ValidaObjetoEqpto(_di_IXMLTEqpto xmlEqpto, int num_coordId);
		bool __fastcall ValidaObjetoID(_di_IXMLTId XMLId);
		bool __fastcall ValidaObjetoLtc( _di_IXMLTLtc xmlLtc);
		bool __fastcall ValidaObjetoMedicao(_di_IXMLTMedicao xmlMedicao);
		bool __fastcall ValidaObjetoPatamar(_di_IXMLTPatamProp xmlPatamar);
      bool __fastcall ValidaObjetoPotCpx(_di_IXMLTPotCpx xmlPotCpx);
		bool __fastcall ValidaObjetoRede(_di_IXMLTRede XMLRede);
      bool __fastcall ValidaObjetoZ(_di_IXMLTZ xmlZ);
		bool __fastcall ValidaObjetoZSeq(_di_IXMLTZseq xmlZSeq);

	private: //métodos
		bool __fastcall ExecutaAvaliacao(void);
		void __fastcall LogAviso(AnsiString txt);
      void __fastcall LogBlocoIni(AnsiString txt);
		void __fastcall LogBlocoFim(void);
		void __fastcall LogMsg(AnsiString txt);
		void __fastcall LogErro(AnsiString txt);
		bool __fastcall ValidaVariavelDouble(AnsiString nome,  double     __fastcall(__closure *funcao)(void));
		bool __fastcall ValidaVariavelInteger(AnsiString nome, int        __fastcall(__closure *funcao)(void));
		bool __fastcall ValidaVariavelString (AnsiString nome, UnicodeString __fastcall(__closure *funcao)(void));

	private: //objetos externos
		VTApl *apl;
		VTLog *plog;
		_di_IXMLTConcessionaria xmlConcessAux;

	private: //dados locais
      int num_tab;
      int num_erro;
      int num_aviso;
   };

#endif
//---------------------------------------------------------------------------
//eof
