// ---------------------------------------------------------------------------
#ifndef TEstilosH
#define TEstilosH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstilos.h"
#include <map>
// ---------------------------------------------------------------------------
class VTFiguraComentario;
class VTLink;
class VTRetangulo;
class VTFiguras;
// ---------------------------------------------------------------------------
using namespace std;
// ---------------------------------------------------------------------------
class TEstilos : public VTEstilos
{
	public:
					__fastcall TEstilos(VTApl *apl);
					__fastcall ~TEstilos(void);
		int  		__fastcall AtributoGraficoId(AnsiString descricao);
		void        __fastcall AtualizaEstilos(void);
		// bool     __fastcall  ExisteAlteracao(void);
		VTEstilo*   __fastcall ExisteEstilo(int estiloId);
		VTEstilo*   __fastcall ExisteEstilo(AnsiString descricao);
		int         __fastcall FormaAtributoId(AnsiString nome);
		int         __fastcall IdEstiloFiguraId(int figuraId);
		VTEstilo* 	__fastcall InsereEstilo(strESTILO *str);
		VTEstilo* 	__fastcall InsereEstilo(VTFigura *figura, AnsiString descricao);
		bool 		__fastcall InsereEstilo(VTEstilo *estilo);
		strATRIBUTO_GRAFICO* __fastcall InsereAtributoGrafico(AnsiString descricao);
		strFORMA_ATRIBUTO*   __fastcall InsereFormaAtributo(AnsiString nome);
		strAUXILIAR_FIGURA*	 __fastcall InsereForma(AnsiString descricao, int id);
		strAUXILIAR_FIGURA*	 __fastcall InsereTipoValorAtributoGrafico(AnsiString descricao, int id);
//		TList*		__fastcall LisEstilo(void);
		bool        __fastcall LeEstilosBase(void);
		void        __fastcall RedefineIdsEstilo(void);
        void        __fastcall ResetAuxiliaresLidos(void);
		bool 		__fastcall RetiraEstilo(VTEstilo *estilo);
		int  		__fastcall TipoFiguraToFormaId(VTFigura *figura);
		void 		__fastcall AssociaEstilos(void); //temporaria
		int  		__fastcall AssociaEstilo(VTFigura *figura); //temporaria

	private:
		AnsiString  __fastcall CriaHash(VTFigura *figura);
		AnsiString  __fastcall CriaString(VTFiguraComentario *comentario);
		AnsiString  __fastcall CriaString(VTRetangulo *retangulo);
		AnsiString  __fastcall CriaString(VTLink *link);
		AnsiString  __fastcall CriaString(TFont *font);
		void 		__fastcall DestroiAuxiliares(void);
		void 		__fastcall DestroiAuxiliares(TList *list);
		strESTILO_ATRIBUTO* __fastcall EstiloAtributo(int estiloAtributoId, strESTILO *str);
		bool        __fastcall PreencheAuxiliares(void);
		bool 		__fastcall PreencheDefaultAtributoGrafico(void);
//		bool 		__fastcall PreencheDefaultEstilo(void);
		bool 		__fastcall PreencheDefaultFormaAtributo(void);
		bool 		__fastcall PreencheDefaultForma(void);
		bool 		__fastcall PreencheDefaultTipoValorAtributoGrafico(void);
		void        __fastcall PreencheEstilo(VTEstilo *estilo, strESTILO *str);
		void        __fastcall PreencheEstilo(VTEstilo *estilo, VTFiguraComentario *comentario);
		void        __fastcall PreencheEstilo(VTEstilo *estilo, VTLink *link);
		void        __fastcall PreencheEstilo(VTEstilo *estilo, VTRetangulo *retangulo);
		VTEstilo* 	__fastcall MapaEstilo(AnsiString descricao);
		int  		__fastcall MapaEstiloId(AnsiString descricao);
		bool        __fastcall ValorBool(strESTILO_ATRIBUTO *ref);
		int         __fastcall ValorInt(strESTILO_ATRIBUTO *ref);

		//Atributos Graficos
		bool __fastcall InsereEstiloAtributoComentario(VTEstilo *estilo, VTFiguraComentario *comentario);
		bool __fastcall InsereEstiloAtributoLink(VTEstilo *estilo, VTLink *link);
		bool __fastcall InsereEstiloAtributoRetangulo(VTEstilo *estilo, VTRetangulo *retangulo);

        //tipo valor atributo grafico

	private:
		// objetos externos
		 VTApl *apl;
	private:
		// dados locais
		AnsiString mErrorFunction;
//		VTFiguras *figuras;
		//guarda os VTEstilo criados
		map<AnsiString,VTEstilo*> mapaEstilo;
		//guarda os Atributo Graficos existentes (default)
		map<AnsiString,strATRIBUTO_GRAFICO*> mapaAtributoGrafico;
        //guarda os tipo de valor atributo grafico existentes (default)
		map<AnsiString,strAUXILIAR_FIGURA*> mapaTipoValorAtributoGrafico;
		//map provisório para guardar descricao-id de tabelas  default
		map<AnsiString,strFORMA_ATRIBUTO*> mapaFormaAtributo;
		map<AnsiString,strAUXILIAR_FIGURA*> mapaForma;
		//listas Auxiliares que um dia devem sair do BD e ir prum BO das figuras

};

#endif
// ---------------------------------------------------------------------------
// eof
