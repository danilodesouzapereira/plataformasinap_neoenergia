//---------------------------------------------------------------------------
#ifndef VTGeraLisParamEqptoH
#define VTGeraLisParamEqptoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTParamConf;
class VTBlocos;
class strDadoParamGerais;
//---------------------------------------------------------------------------
class VTGeraLisParamEqpto: public TObject
	{
	public:  //property
		  __property TList*           LisBarraOc          = {read=PM_GetLisBarraOc}; //hk201703: adaptacoes planTurmas
		  TList  *lisChavesProtMont;
		  TList	*lisChavesMont;

	public:
									__fastcall VTGeraLisParamEqpto(void) {};
									__fastcall ~VTGeraLisParamEqpto(void) {};
		  virtual    	 bool __fastcall GeraLisParamEqptoBdado(TList *lisOc, TList *lisEqptoOc, TList *lisParamEqpto, int ndiasObsExt = -1) = 0;
		  virtual    	 bool __fastcall GeraLisBarraOcBdado(TList *lisEqptoOc, TList *lisBarraOc) = 0;//hk201702: integração simTurmas
		  virtual    	 void __fastcall PreparaLisParamEqptoRede(TList *lisParamEqptoExterno, double *distFalhas = NULL) = 0;
    virtual void    __fastcall ReiniciaLisParamBlocoRede(TList       *lisParamEqptoEntrada,
                                                         TList       *lisTipoEqProt,
                                                         TList       *lisParamBlocoExterno,
                                                         TList       *lisParamCargaExterno,
                                                         VTBlocos    *blocos) = 0;
		  virtual		 void __fastcall PreparaParamConfRede(VTParamConf *paramConf, VTBlocos *blocos = NULL) = 0;
		  virtual		 void __fastcall PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg = false) = 0; //hk201607 Virou função externa, acrescimo de flag
		  virtual		 void __fastcall PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg = false) = 0; //hk201607 Virou função externa, acrescimo de flag
		  //hk201611: mudanca dos parametros virtual		 void __fastcall CalculaDadosParamRede(VTParamConf *paramConf) = 0;//(TList *lisParamEqpto, TList *lisParamRede) = 0;
		  virtual		 void __fastcall CalculaDadosParamRede(TList *lisParamEqpto, TList *lisParamRede, strDadoParamGerais *paramGeral) = 0;//(TList *lisParamEqpto, TList *lisParamRede) = 0;
		  virtual		 void __fastcall ReiniciaDadosParamEqpto(TList *lisParamEqpto) = 0;
		  virtual		 void __fastcall ReiniciaDadosParamRede(TList *lisParamRede) = 0;
		  virtual		 void __fastcall ReiniciaDadosParamGeral(strDadoParamGerais *paramGeral) = 0;
		  virtual		 void __fastcall ReiniciaDistribHora(strDadoParamGerais *paramGeral, double *distFalhasPat) = 0;
		  virtual		 void __fastcall RedistribuiFalhasCirc(VTParamConf *paramConf) = 0;
		  virtual		 int  __fastcall RedistribuiFalhasCircMes(VTParamConf *paramConf) = 0; //retorna número de meses com falhas
		  virtual		 void __fastcall ImprimeLisParamEqpto(TList *lisParamEqpto, AnsiString nomeArq) = 0; //hk201609
		  //virtual		 void __fastcall RedistribuiFalhasCirc(TList *lisParamEqpto, TList *lisParamBloco, TList *lisParamCarga) = 0;
        virtual		 void __fastcall PreparaLisChavesIF(TList *lisChavesIFExterno) = 0;//hk201808: reformulação

	protected: //métodos acessados via property
		  virtual     TList* __fastcall PM_GetLisBarraOc(void) = 0;  //hk201703: adaptacoes planTurmas


	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTGeraLisParamEqpto* __fastcall NewObjGeraLisParamEqpto(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
