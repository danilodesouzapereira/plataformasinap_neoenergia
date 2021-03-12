//---------------------------------------------------------------------------
#ifndef VTItemObraH
#define VTItemObraH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTObra;
class VTIpeAtributo;
//---------------------------------------------------------------------------
class VTItemObra : public TObject
{
	public:  //property
		__property int        Id                    = {read=PD.id,                		write=PD.id};
		__property int        TipoAcao              = {read=PD.tipoAcao,              	write=PD.tipoAcao};
		__property TList*     LisIpeAtributo 		= {read=PD.lisIpeAtributo};
		__property AnsiString Nome                  = {read=PD.nome,              		write=PD.nome};
		__property bool 	  Configurado		    = {read=PD.configurado,             write=PD.configurado};
		__property bool 	  Contabilizar	     	= {read=PD.contabilizar,           write=PD.contabilizar};
//		__property double     ObEspecial            = {read=PD.ob_esp,        			write=PD.ob_esp};
//		__property double     PartOtr               = {read=PD.part_otr,           		write=PD.part_otr};
//		__property int        AnoAtivacao           = {read=PM_GetAnoAtivacao        };
//		__property int        AnoInicio             = {read=PM_GetAnoInicio          };
//		__property TDateTime  DataAtivacao          = {read=PM_GetDataAtivacao,      write=PM_SetDataAtivacao};
//		__property TDateTime  DataInicio            = {read=PM_GetDataInicio,        write=PM_SetDataInicio};
//		__property AnsiString Responsavel           = {read=PM_GetResponsavel,       write=PM_SetResponsavel};
		__property VTObra*    Obra                  = {read=PD.obra,             		write=PD.obra};
//		__property bool       Visible               = {read=PD.visible,           		write=PD.visible};
//		__property bool       Incorporada           = {read=PD.incorporada,       		write=PD.incorporada};
		__property TList*     LisAcao               = {read=PD.lisACAO};
//		__property int        ModalResult           = {read=PD.modal_result,       		write=PD.modal_result};
//		__property TColor     Color                 = {read=PD.color,             		write=PD.color};
//		__property bool       ColorEnabled          = {read=PD.color_enabled,      		write=PD.color_enabled};
		__property double     ReducaoPNT		    = {read=PD.reducaoPNT_kwh_mes, 	write=PD.reducaoPNT_kwh_mes};
		__property bool       AlteraTaxaFalha       = {read=PD.altera_taxa_falha,   	write=PD.altera_taxa_falha};
		__property double     TaxaFalhaInsercao     = {read=PD.taxa_falha_insercao, 	write=PD.taxa_falha_insercao};
		__property double     TaxaFalhaSubstituicao = {read=PD.taxa_falha_substituicao, write=PD.taxa_falha_substituicao};
		__property TObject*   Object                = {read=PD.object, write=PD.object};

	public:
					__fastcall  VTItemObra(void) {};
	virtual 		__fastcall ~VTItemObra(void) {};
	virtual VTItemObra* __fastcall  Clone(bool preserveId, bool isDeep) =0;
	virtual void    __fastcall  CorrigeValores(int tipoEqpto) = 0;
	virtual void    __fastcall  CopiaAtributosDe(VTItemObra &ref)=0;
	virtual void    __fastcall  IniciaExtras(void)=0;
	virtual void    __fastcall 	InsereAcao(VTAcao *acao) = 0;
	virtual void    __fastcall 	InsereIpeAtributo(VTIpeAtributo *atributo)=0;
	virtual	void    __fastcall  LeIpeAtributos(void) = 0;
	virtual void    __fastcall  LeIpeAtributo(VTIpeAtributo *atributo) =0;
	virtual void    __fastcall  MantemAtributosComunsDe(VTItemObra &ref)=0;
	virtual void    __fastcall  SalvaIpeAtributos(void) = 0;
	virtual void    __fastcall  SalvaIpeAtributosProprios(void) = 0;
	protected:  //
		VTApl *apl;

		struct
		{
			VTObra     *obra;
			int        id;
			int        tipoAcao;
			AnsiString nome;
//			AnsiString responsavel;
//			TDateTime  data_inicio;
//			TDateTime  data_ativacao;
			bool       configurado;
			bool       contabilizar;
//			double     ob_esp;     //obrigação especial (%)
//			double     part_otr;   //participação OTR (%)
//			bool       visible;
//			bool       fixa;
//			bool       incorporada;
//			int        modal_result;
//			TColor     color;
//			bool       color_enabled;
			double     reducaoPNT_kwh_mes;
			//dados para cálculo de continuidade
			bool       altera_taxa_falha;
			double     taxa_falha_insercao;
			double     taxa_falha_substituicao;
			TList 		*lisACAO;
			TObject     *object;
			TList       *lisIpeAtributo;
		}PD;



};

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe VTItemObra
//---------------------------------------------------------------------------
VTItemObra* NewObjItemObra(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof