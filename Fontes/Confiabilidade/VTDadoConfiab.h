//---------------------------------------------------------------------------
#ifndef VTDadoConfiabH
#define VTDadoConfiabH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTConfiab.h"
//#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTDataEqpto;
class VTBloco;
class VTCabo;
class VTRede;
class VTTipoChave;
//---------------------------------------------------------------------------
class VTDadoConfiab : public TObject
   {
	public:  //property
		__property VTBloco*      Bloco               = {read=PM_GetBloco};
		__property VTDataEqpto*  DataGeralTrecho     = {read=PM_GetDataGeralTrecho};
		__property VTDataEqpto*  DataGeralEqpto      = {read=PM_GetDataGeralEqpto};
		__property TList*        LisDataEqpto        = {read=PM_GetLisDataEqpto};
		__property TList*        LisDataTipo         = {read=PM_GetLisDataTipo};
		__property TList*        LisDataTipoTrecho   = {read=PM_GetLisDataTipoTrecho};
		__property TList*        LisDataTipoChave    = {read=PM_GetLisDataTipoChave};
		__property TList*        LisDataTipoGerador  = {read=PM_GetLisDataTipoGerador};
		__property TList*        LisDataTipoTrafo    = {read=PM_GetLisDataTipoTrafo};
		__property TList*        LisDataBloco        = {read=PM_GetLisDataBloco};
		__property TList*        LisDataRede         = {read=PM_GetLisDataRede};
		__property TList*        LisDataAjusteRede   = {read=PM_GetLisDataAjusteRede};
		__property TList*        LisEqptoMutua       = {read=PM_GetLisEqptoMutua};

	public:
                         __fastcall VTDadoConfiab(void) {};
		virtual            __fastcall ~VTDadoConfiab(void) {};
		virtual  void      __fastcall CopiaDados(VTDadoConfiab* dataExt) = 0;
		virtual  void      __fastcall DefParamEqpto(PAR_CONF pconf, VTEqpto* eqpto) = 0;
      virtual  void      __fastcall DefParamGeralEqpto(PAR_CONF pconf) = 0;
		virtual  void      __fastcall DefParamGeralTrecho(PAR_CONF pconf) = 0;
		virtual  void      __fastcall DefParamGeralTipoChave(PAR_CONF pconf, TObject* tipochave) = 0;
		virtual  void      __fastcall DefParamGeralTipoGerador(PAR_CONF pconf, double smin, double smax) = 0;
		virtual  void      __fastcall DefParamGeralTipoTrafo(PAR_CONF pconf, double smin, double smax) = 0;
		virtual  VTDataEqpto*      __fastcall DefParamGeralTipoTrecho(PAR_CONF pconf, VTEqpto* cabo) = 0;
		virtual  void      __fastcall DefParamGeralTipo(PAR_CONF pconf, int itipo) = 0;
		virtual  void      __fastcall DefParamFalhaRede(PAR_CONF pconf, TObject* rede) = 0;
		virtual  void      __fastcall DefParamRede(RES_CONF pres, VTRede* rede) = 0;
		virtual  void      __fastcall Inicia(void) = 0;
		virtual  void      __fastcall IniciaFatRede(void) = 0;
		virtual  void      __fastcall ImprimeDado(FILE *fout) = 0;
		virtual  bool      __fastcall Interrupcoes(VTEqpto* eqpto, double &fano, double &mano) = 0;
      virtual  bool      __fastcall ParamEqpto(PAR_CONF &pconf, VTEqpto* eqpto) = 0;
      virtual  bool      __fastcall ParamGeralEqpto(PAR_CONF &pconf) = 0;
      virtual  bool      __fastcall ParamGeralTrecho(PAR_CONF &pconf) = 0;
		virtual  bool      __fastcall ParamGeralTipo(PAR_CONF &pconf, int itipo) = 0;
		virtual  bool      __fastcall ParamTipoTrecho(PAR_CONF &pconf, VTCabo* cabo) = 0;
		virtual  bool      __fastcall ParamTipoChave(PAR_CONF &pconf, VTTipoChave* ptipo) = 0;
		virtual  bool      __fastcall ParamTipoGerador(PAR_CONF &pconf, double s) = 0;
		virtual  bool      __fastcall ParamTipoTrafo(PAR_CONF &pconf, double s) = 0;

	protected:  //métodos acessados via property
		virtual VTBloco*     __fastcall PM_GetBloco(void) = 0;
		virtual VTDataEqpto* __fastcall PM_GetDataGeralTrecho(void) = 0;
		virtual VTDataEqpto* __fastcall PM_GetDataGeralEqpto(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataEqpto(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataTipo(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataTipoTrecho(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataTipoChave(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataTipoGerador(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataTipoTrafo(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataBloco(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataRede(void) = 0;
		virtual TList*       __fastcall PM_GetLisDataAjusteRede(void) = 0;
	   virtual TList*       __fastcall PM_GetLisEqptoMutua(void) = 0;

	public: //dados
		AnsiString  descricao;
		/*
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
		*/

		//Parametros para o calculo dos indices de confiabilidade
		int  icont;//ordem das contingencias considerado
		bool flag_NA;//consideração de socorro entre circuitos
		bool flag_BLOCO;//calculo agregado por blocos
		bool flag_BARRA;//se são consideradas falhas em barras
		//Tipos de chave de protecao
		TList      *lisTipoChv;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTChave
//---------------------------------------------------------------------------
VTDadoConfiab* __fastcall NewObjDadoConfiab(void);

//---------------------------------------------------------------------------
#endif
//eof


 