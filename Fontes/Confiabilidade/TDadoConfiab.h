//---------------------------------------------------------------------------
#ifndef TDadoConfiabH
#define TDadoConfiabH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDadoConfiab.h"
//---------------------------------------------------------------------------
class TDadoConfiab : public VTDadoConfiab
   {
	public:
					__fastcall TDadoConfiab(void);
					__fastcall ~TDadoConfiab(void);
	  void      __fastcall CopiaDados(VTDadoConfiab* dataExt);
	  void      __fastcall DefParamEqpto(PAR_CONF pconf, VTEqpto* eqpto);
	  void      __fastcall DefParamGeralEqpto(PAR_CONF pconf);
	  void      __fastcall DefParamGeralTrecho(PAR_CONF pconf);
	  void      __fastcall DefParamGeralTipoChave(PAR_CONF pconf, TObject* tipochave);
	  void      __fastcall DefParamGeralTipoGerador(PAR_CONF pconf, double smin, double smax);
	  void      __fastcall DefParamGeralTipoTrafo(PAR_CONF pconf, double smin, double smax);
	  VTDataEqpto*      __fastcall DefParamGeralTipoTrecho(PAR_CONF pconf, VTEqpto* cabo);
	  void      __fastcall DefParamGeralTipo(PAR_CONF pconf, int itipo);
	  void      __fastcall DefParamFalhaRede(PAR_CONF pconf, TObject* rede);
	  void      __fastcall DefParamRede(RES_CONF pres, VTRede* rede);

	  void      __fastcall Inicia(void);
	  void      __fastcall IniciaFatRede(void);
	  void      __fastcall ImprimeDado(FILE *fout);
	  bool      __fastcall Interrupcoes(VTEqpto* eqpto, double &fano, double &mano);

	  bool      __fastcall ParamEqpto(PAR_CONF &pconf, VTEqpto* eqpto);
	  bool      __fastcall ParamGeralEqpto(PAR_CONF &pconf);
	  bool      __fastcall ParamGeralTrecho(PAR_CONF &pconf);
	  bool      __fastcall ParamGeralTipo(PAR_CONF &pconf, int itipo);
	  bool      __fastcall ParamTipoTrecho(PAR_CONF &pconf, VTCabo* cabo);
	  bool      __fastcall ParamTipoChave(PAR_CONF &pconf, VTTipoChave* ptipo);
	  bool      __fastcall ParamTipoGerador(PAR_CONF &pconf, double s);
	  bool      __fastcall ParamTipoTrafo(PAR_CONF &pconf, double s);

	private: //métodos
	  bool      __fastcall Param1Eqpto(PAR_CONF &pconf, VTEqpto* eqpto);
	  bool      __fastcall ParamTipoEqpto(PAR_CONF &pconf, VTEqpto* eqpto);
	  int       __fastcall TipoEqpto(VTEqpto* eqpto);

	protected: //métodos acessados via property
	  VTBloco*     __fastcall PM_GetBloco(void) {return(bloco);};
	  VTDataEqpto* __fastcall PM_GetDataGeralTrecho(void) {return(dataGeralTrecho);};
	  VTDataEqpto* __fastcall PM_GetDataGeralEqpto(void) {return(dataGeralEqpto);};
	  TList*       __fastcall PM_GetLisDataEqpto(void) {return(lisDataEqpto);};
	  TList*       __fastcall PM_GetLisDataTipo(void) {return(lisDataTipo);};
	  TList*       __fastcall PM_GetLisDataTipoTrecho(void) {return(lisDataTipoTrecho);};
	  TList*       __fastcall PM_GetLisDataTipoChave(void) {return(lisDataTipoChave);};
	  TList*       __fastcall PM_GetLisDataTipoGerador(void) {return(lisDataTipoGerador);};
	  TList*       __fastcall PM_GetLisDataTipoTrafo(void) {return(lisDataTipoTrafo);};
	  TList*       __fastcall PM_GetLisDataBloco(void) {return(lisDataBloco);};
	  TList*       __fastcall PM_GetLisDataRede(void) {return(lisDataFalhaRede);};
	  TList*       __fastcall PM_GetLisDataAjusteRede(void) {return(lisDataRede);};
	  TList*       __fastcall PM_GetLisEqptoMutua(void) {return(lisEqptoMutua);};


	private: //dados externos

	private: //dados locais
		//Dados por equipamento e tipo de equipamento
		VTDataEqpto *dataGeralTrecho;
		VTDataEqpto *dataGeralEqpto;
		TList      *lisDataEqpto;
		TList      *lisDataTipo;
		TList      *lisDataTipoTrecho;
		TList      *lisDataTipoChave;
		TList      *lisDataTipoGerador;
		TList      *lisDataTipoTrafo;
		TList      *lisDataFalhaRede;
		//Lista de VTDadoGeral que aponta para bloco c/ parametro específico
		TList      *lisDataBloco;
		VTBloco    *bloco;  //bloco com parametros específicos

		//Ajuste de indicadores por rede
		TList      *lisDataRede;

      TList      *lisEqptoMutua;
	public:

	};

#endif
//---------------------------------------------------------------------------
//eof
