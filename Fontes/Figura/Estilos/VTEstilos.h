// ---------------------------------------------------------------------------
#ifndef VTEstilosH
#define VTEstilosH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTEstilo;
class VTFigura;
//---------------------------------------------------------------------------
//Constantes
//---------------------------------------------------------------------------
//estilo
#define AUXF_ESTILO_DEFAULT "Default"
//formas
#define AUXF_FORMA_RETANGULO "Retângulo"
//formaAtributos
#define AUXF_FORMAAT_DIMX "Largura do retângulo em pixels"
#define AUXF_FORMAAT_DIMY "Altura do retângulo em pixels"
//tipoValorAtributoGrafico
#define AUXF_TVAG_INT    "Valor inteiro"
#define AUXF_TVAG_DOUBLE "Valor de ponto flutuante (dupla precisão)"
#define AUXF_TVAG_STRING "Texto"
//atributoGrafico
#define AUXF_AG_BORDATIPO      "Tipo de traço da borda"
#define AUXF_AG_BORDAESPESSURA "Espessura do traço da borda em pixels"
#define AUXF_AG_BORDACOR       "Cor do traço da borda"
#define AUXF_AG_FUNDOCOR       "Cor do fundo"
#define AUXF_AG_TRANSPARENTE   "Se o fundo é transparente"
#define AUXF_AG_FONTSIZE       "Tamanho da FONT"
#define AUXF_AG_FONTCOLOR      "Cor da FONT"
#define AUXF_AG_FONTBOLD       "FONT em negrito"
#define AUXF_AG_FONTITALIC     "FONT em itálico"
#define AUXF_AG_FONTUNDERLINE  "FONT com sublinhado"
#define AUXF_AG_FONTSTRIKEOUT  "FONT tachada"
#define AUXF_AG_FONTNAME       "Nome da FONT"
//---------------------------------------------------------------------------
enum estiloATRIBUTOGRAFICO {//tipo de atributo
				 //int
				 eatBORDA_TIPO=1,
				 eatBORDA_ESPESSURA,
				 eatBORDA_COR,
				 eatFUNDO_COR,
				 eatFUNDO_TRANSPARENTE,
				 eatFONT_SIZE,
				 eatFONT_COLOR,
				 eatFONT_BOLD,
				 eatFONT_ITALIC,
				 eatFONT_UNDERLINE,
				 eatFONT_STRIKEOUT,
				 //strings
				 eatFONT_NAME,
				 eatCOUNT
			   };
//---------------------------------------------------------------------------
class strAUXILIAR_FIGURA
{
	public:
		int id;
		AnsiString descricao;
        bool salvoNaBase;
};
//---------------------------------------------------------------------------
class strESTILO_ATRIBUTO : public strAUXILIAR_FIGURA
{
	public:
		int atributoGraficoId;
		int estiloId;
		AnsiString valor;
};
//---------------------------------------------------------------------------
//???
class strESTILO : public strAUXILIAR_FIGURA
{
	public:
		DynamicArray<strESTILO_ATRIBUTO> atributos;
};
//---------------------------------------------------------------------------
class strATRIBUTO_GRAFICO : public strAUXILIAR_FIGURA
{
	public:
		int tipoValorAG;
};
//---------------------------------------------------------------------------
class strFORMA_ATRIBUTO : public strAUXILIAR_FIGURA
{
	public:
		int formaId;
		AnsiString nome;
		AnsiString tipoValor;
};
//---------------------------------------------------------------------------
class strTEXTO  : public strAUXILIAR_FIGURA
{
	public:
		int figuraId;
		int estiloId;
		AnsiString texto;
};
//---------------------------------------------------------------------------
//essa deve ir pro VTFiguras
class strVALOR_FORMA_ATRIBUTO  : public strAUXILIAR_FIGURA
{
	public:
        int figuraId;
		int formaAtributoId;
		AnsiString valor;
};
// ---------------------------------------------------------------------------
class VTEstilos : public TObject
{
	public:
		__property TList*   LisEstilo  					= {read=PD.lisESTILO};
		__property TList*   LisAtributoGrafico  		= {read=PD.lisAtributoGrafico};
		__property TList*   LisFormaAtributo  			= {read=PD.lisFormaAtributo};
		__property TList*   LisForma  					= {read=PD.lisForma};
		__property TList*   LisTipoValorFormaAtributo	= {read=PD.lisTipoValorAtributoGrafico};


	public:
							 __fastcall  VTEstilos(void){};
							 __fastcall ~VTEstilos(void){};
		virtual int  		 __fastcall AtributoGraficoId(AnsiString descricao) =0;
		virtual void         __fastcall AtualizaEstilos(void) = 0;
		virtual VTEstilo*    __fastcall ExisteEstilo(int estiloId) = 0;
		virtual VTEstilo*    __fastcall ExisteEstilo(AnsiString descricao) = 0;
		virtual VTEstilo* 	 __fastcall MapaEstilo(AnsiString descricao) = 0;
        virtual int          __fastcall FormaAtributoId(AnsiString nome)=0;
		virtual int          __fastcall IdEstiloFiguraId(int figuraId) = 0;
		virtual VTEstilo* 	 __fastcall InsereEstilo(strESTILO *str) = 0;
		virtual VTEstilo* 	 __fastcall InsereEstilo(VTFigura *figura, AnsiString descricao = "") = 0;
		virtual bool 		 __fastcall InsereEstilo(VTEstilo *estilo) = 0;
        virtual bool         __fastcall LeEstilosBase(void) = 0;
		virtual bool 		 __fastcall PreencheDefaultAtributoGrafico(void)=0;
		virtual bool 		 __fastcall PreencheDefaultFormaAtributo(void)=0;
		virtual bool 		 __fastcall PreencheDefaultForma(void)=0;
		virtual bool 		 __fastcall PreencheDefaultTipoValorAtributoGrafico(void) =0;
		virtual void         __fastcall RedefineIdsEstilo(void) = 0;
		virtual void         __fastcall ResetAuxiliaresLidos(void) = 0;
		virtual bool         __fastcall RetiraEstilo(VTEstilo *estilo) =0 ;
        virtual int  		 __fastcall TipoFiguraToFormaId(VTFigura *figura) =0;
		virtual void 		 __fastcall AssociaEstilos(void)=0; //temporaria
		virtual int  		 __fastcall AssociaEstilo(VTFigura *figura)=0; //temporaria
		//insere estruturas auxiliares
		virtual strATRIBUTO_GRAFICO* __fastcall InsereAtributoGrafico(AnsiString descricao) = 0;
		virtual strFORMA_ATRIBUTO*   __fastcall InsereFormaAtributo(AnsiString nome) = 0;
		virtual strAUXILIAR_FIGURA*	 __fastcall InsereForma(AnsiString descricao, int id) = 0;
		virtual strAUXILIAR_FIGURA*	 __fastcall InsereTipoValorAtributoGrafico(AnsiString descricao, int id) = 0;

	protected:
		struct
		{
			TList *lisESTILO;
			TList *lisAtributoGrafico;
			TList *lisFormaAtributo;
			TList *lisForma;
			TList *lisTipoValorAtributoGrafico;
		}PD;
};
//---------------------------------------------------------------------------
VTEstilos* __fastcall NewObjEstilos(VTApl *apl);
#endif
// ---------------------------------------------------------------------------
// eof
