//---------------------------------------------------------------------------
#ifndef TParamConfH
#define TParamConfH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTParamConf.h"

//---------------------------------------------------------------------------
class TParamConf : public VTParamConf
	{
	public:
							  __fastcall TParamConf(void);
							  __fastcall ~TParamConf(void);
		VTParamConf*     __fastcall Clone(VTParamConf* parConfOrig);
		VTVerifCircuito* __fastcall CriaVerifCircuito(void);
		VTVerifCjto*     __fastcall CriaVerifCjto(void);
		VTVerifSe*       __fastcall CriaVerifSe(void);
		VTVerifCircuito* __fastcall ExisteVerifCircuito(AnsiString codigo);
		VTVerifSe*       __fastcall ExisteVerifSe(AnsiString codigo);
		void             __fastcall ImprimeParametrosSimula(AnsiString arq);
		void             __fastcall ImprimeParametrosEntrada(AnsiString arq);
		void             __fastcall ImprimeParametrosMes(AnsiString arq);
		void             __fastcall IniciaParamDefault(void);
		void             __fastcall InsereParamBloco(VTBloco *bloco, double lambda, double trep); //hk201702: inserção de bloco
      //hkreformula
      void             __fastcall InsereLisTipoChaveProtecaoParamConf(TList *lisTipoEqProtExterno);
      void             __fastcall IniciaLisParamRede(TList *lisRede);
      void             __fastcall IniciaDistFalhas(int numPat);
      void             __fastcall RedistribuiFalhasHora(int numPat); //hkreformula:
      bool             __fastcall GravaArqMdb(VTApl *apl, AnsiString arqParam);  //hkreformula:
      void             __fastcall GravaDirConf(VTApl *apl);//hkreformula
    strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTChave *chave);//hkreformula
    strDadoParamEqpto* __fastcall ParametroDadoEqpto(AnsiString codChave,
	                               AnsiString codRede);//hkreformula
    strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTCarga *carga);//hkreformula
      bool             __fastcall LeArquivosMdb(VTApl* apl, AnsiString pathParam);//hkreformula
    AnsiString         __fastcall LePathParam(VTApl* apl); //hk201808: reformula vai para bdadoParam
	private: //métodos
		double           __fastcall ComprimentoBloco(VTBloco *bloco);//hk201702
      double           __fastcall ComprimentoRede(VTRede *rede);
      bool             __fastcall LeArqMdb(VTApl *apl, AnsiString arqParam); //hkreformula
    AnsiString         __fastcall LeDado(FILE* fp, bool &flagEOF);


	protected:  //métodos acessados via property
		strDadoParamGerais* __fastcall PM_GetParamGerais(void);
		strDadoEqptoIndisp* __fastcall PM_GetEqptoIndispPadrao(void) {return(eqptoIndispPadrao);};//hk201808: indisp
		strDadoParamBloco*  __fastcall PM_GetParamBloco(VTBloco *bloco);
		strDadoParamCarga*  __fastcall PM_GetParamCarga(VTCarga *carga);
		strDadoParamRede*   __fastcall PM_GetParamRede(VTRede *rede);
		strDadoParamRede*   __fastcall PM_GetParamCodRede(AnsiString codRede);
		TList*              __fastcall PM_GetLisParamBloco(void);
		TList*              __fastcall PM_GetLisParamCarga(void);
		TList*              __fastcall PM_GetLisParamEqpto(void);
		TList*              __fastcall PM_GetLisParamRede(void);
		TList*              __fastcall PM_GetLisVerifCircuito(void);
		TList*              __fastcall PM_GetLisVerifCjto(void);
		TList*              __fastcall PM_GetLisVerifSe(void);
		TList*              __fastcall PM_GetLisChavesTelecom(void);
		TList*              __fastcall PM_GetLisChavesIF(void);
		TList*              __fastcall PM_GetLisTipoEqProt(void);
		TList*              __fastcall PM_GetLisEqptoIndisp(void) {return(lisEqptoIndisp);}; //hk201808: indisp
		double*             __fastcall PM_GetDistFalhasPat(void) {return(distFalhasPat);};
		AnsiString          __fastcall PM_GetPathParam(void) {return(pathParam);};//hk201808: reformula

	private: //dados acessados via property

	private: //dados locais
		strDadoParamGerais *paramGerais;
		TList *lisParamBloco;
		TList *lisParamCarga;
		TList *lisParamRede;
		TList *lisParamEqpto;
		TList *lisChavesTelecom; //hk20
		TList *lisChavesIF;
		TList *lisVerifCircuito;
		TList *lisVerifCjto;
		TList *lisVerifSe;
		TList *lisTipoEqProt;    //hk21 tipos de equipamento de protecao
      TList *lisEqptoIndisp;   //hk201808: indisp
      strDadoEqptoIndisp *eqptoIndispPadrao; //hk201808: indisp
		double* distFalhasPat;

      //hk201808: reformula
      AnsiString pathParam;
	};

#endif
//---------------------------------------------------------------------------
