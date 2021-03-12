//---------------------------------------------------------------------------
#ifndef VTBarraOcH
#define VTBarraOcH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <TeEngine.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTEqpto;
class VTAbrangencia;
class VTRede;
class VTLocalidade;
class VTPolo;
class VTEqptoOc;
//---------------------------------------------------------------------------
class VTBarraOc : public TObject
	{
	public: //property
		__property AnsiString     Codigo         = {read=PM_GetCodigo, write=PM_SetCodigo};
		__property AnsiString     CodigoRede     = {read=PM_GetCodigoRede, write=PM_SetCodigoRede};
		__property VTRede*        RedePri        = {read=PM_GetRedePri, write=PM_SetRedePri};
		__property VTEqpto*       EqptoOcorr     = {read=PM_GetEqOcorr, write=PM_SetEqOcorr};
		__property TObject*       ParEqExt       = {read=PM_GetParEqptoExt, write=PM_SetParEqptoExt};
		__property TList*         LisTrataOcorr  = {read=PM_GetLisTrataOcorr};
		__property TList*         LisOcorrF      = {read=PM_GetLisOcorrF};
		__property TList*         LisCircuitos   = {read=PM_GetLisCircuitos};
		__property double         NConsBT        = {read=PM_GetNConsBT, write=PM_SetNConsBT};  //hknova
		//__property int            NOcor          = {read=PM_GetNOcor};
		__property double         NOcorRamal     = {read=PM_GetNOcorRamal, write=PM_SetNOcorRamal};
		__property double         NOcorNRamal    = {read=PM_GetNOcorNRamal, write=PM_SetNOcorNRamal};
		__property double         NOcorPond      = {read=PM_GetNOcorPond, write=PM_SetNOcorPond};
		__property double         NDiasObs       = {read=PM_GetNDiasObs, write=PM_SetNDiasObs};
		__property double         DuraTotal      = {read=PM_GetDuraTotal, write=PM_SetDuraTotal};
		__property double         Cli            = {read=PM_GetCli, write=PM_SetCli};
		__property double         Clih           = {read=PM_GetClih, write=PM_SetClih};
		__property double         TdeslMedio     = {read=PM_GetTdeslMedio, write=PM_SetTdeslMedio}; //tempo de desligamento médio
		__property VTEqptoOc*     EqptoOc        = {read=PM_GetEqptoOc, write=PM_SetEqptoOc}; //tempo de desligamento médio
		__property double         NFHora[int n]  = {read=PM_GetNFHora, write=PM_SetNFHora};
		__property double         NFMes[int n]   = {read=PM_GetNFMes, write=PM_SetNFMes};
		__property double         NFRMes[int n]  = {read=PM_GetNFRMes, write=PM_SetNFRMes};
		__property double         CLIHMes[int n] = {read=PM_GetCLIHMes, write=PM_SetCLIHMes};
		__property double         CLIMes[int n]  = {read=PM_GetCLIMes, write=PM_SetCLIMes};
		__property double         TDMes[int n]   = {read=PM_GetTDMes, write=PM_SetTDMes};
		__property double         DuraTotMes[int n] = {read=PM_GetDuraTotalMes, write=PM_SetDuraTotalMes};
		__property double         X                 = {read=PM_GetX, write=PM_SetX}; //hk201702 coordenadas da chave ou da carga
		__property double         Y                 = {read=PM_GetY, write=PM_SetY}; //hk201702 coordenadas da chave ou da carga
		__property TList*         LisParamCarga     = {read=PM_GetLisParamCarga};//hk201703: adaptacoes planTurmas
		__property TList*         LisParamBloco     = {read=PM_GetLisParamBloco};//hk201703: adaptacoes planTurmas
		__property double         Tat               = {read=PM_GetTat, write=PM_SetTat}; //hk201703: Correção do tempo de atendimento
		__property double         TMABase           = {read=PM_GetTMABase, write=PM_SetTMABase};//hk201706: multiplas bases módulo de planejamento de turmas
		__property double         TMAReal           = {read=PM_GetTMAReal, write=PM_SetTMAReal};//hk201706: multiplas bases módulo de planejamento de turmas
		__property double         Ptpre             = {read=PM_GetPtpre, write=PM_SetPtpre}; //hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		__property double         Ptdlo             = {read=PM_GetPtdlo, write=PM_SetPtdlo}; //hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		__property double         Ptexe             = {read=PM_GetPtexe, write=PM_SetPtexe}; //hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)

	public:
										__fastcall VTBarraOc(void) {};
		virtual                 __fastcall ~VTBarraOc(void) {};

	public: //dados locais

	protected:  //métodos acessados via property
		virtual AnsiString      __fastcall PM_GetCodigo(void) = 0;
		virtual void            __fastcall PM_SetCodigo(AnsiString codigo) = 0;
		virtual AnsiString      __fastcall PM_GetCodigoRede(void) = 0;
		virtual void            __fastcall PM_SetCodigoRede(AnsiString codigoRede) = 0;
		virtual VTRede*         __fastcall PM_GetRedePri(void) = 0;
		virtual void            __fastcall PM_SetRedePri(VTRede *rede)  = 0;
		virtual VTEqpto*        __fastcall PM_GetEqOcorr(void)  = 0;
		virtual void            __fastcall PM_SetEqOcorr(VTEqpto *eqpto)  = 0;
		virtual TObject*        __fastcall PM_GetParEqptoExt(void) = 0;
		virtual void            __fastcall PM_SetParEqptoExt(TObject *parEqptoExt) = 0;
		virtual TList*          __fastcall PM_GetLisTrataOcorr(void) = 0;
		virtual TList*          __fastcall PM_GetLisOcorrF(void) = 0;
		virtual TList*          __fastcall PM_GetLisCircuitos(void) = 0;
		//virtual int             __fastcall PM_GetNOcor(void) = 0;
		virtual double          __fastcall PM_GetNOcorRamal(void) = 0;
		virtual void            __fastcall PM_SetNOcorRamal(double nocorRamal) = 0;
		virtual double          __fastcall PM_GetNOcorNRamal(void) = 0;
		virtual void            __fastcall PM_SetNOcorNRamal(double nocorNRamal) = 0;
		virtual double          __fastcall PM_GetNOcorPond(void) = 0;
		virtual void            __fastcall PM_SetNOcorPond(double nocorPond) = 0;
		virtual double          __fastcall PM_GetNDiasObs(void) = 0;
		virtual void            __fastcall PM_SetNDiasObs(double ndiasObs) = 0;
		virtual double          __fastcall PM_GetDuraTotal(void) = 0;
		virtual void            __fastcall PM_SetDuraTotal(double duraTotal)  = 0;
		virtual double          __fastcall PM_GetCli(void) = 0;
		virtual void            __fastcall PM_SetCli(double cli) = 0;
		virtual double          __fastcall PM_GetClih(void) = 0;
		virtual void            __fastcall PM_SetClih(double clih) = 0;
		virtual double          __fastcall PM_GetNConsBT(void) = 0;
		virtual void            __fastcall PM_SetNConsBT(double nconsBT) = 0;
		virtual double          __fastcall PM_GetTdeslMedio(void) = 0;
		virtual void            __fastcall PM_SetTdeslMedio(double tdeslMedio) = 0;
		virtual VTEqptoOc*      __fastcall PM_GetEqptoOc(void) = 0;
		virtual void            __fastcall PM_SetEqptoOc(VTEqptoOc* eqptoOc) = 0;
		virtual double          __fastcall PM_GetNFHora(int n) = 0;
		virtual void            __fastcall PM_SetNFHora(int n, double nf) = 0;
		virtual double          __fastcall PM_GetNFMes(int n) = 0;
		virtual void            __fastcall PM_SetNFMes(int n, double nf) = 0;
		virtual double          __fastcall PM_GetNFRMes(int n) = 0;
		virtual void            __fastcall PM_SetNFRMes(int n, double nf) = 0;
		virtual double          __fastcall PM_GetCLIHMes(int n) = 0;
		virtual void            __fastcall PM_SetCLIHMes(int n, double clih) = 0;
		virtual double          __fastcall PM_GetCLIMes(int n) = 0;
		virtual void            __fastcall PM_SetCLIMes(int n, double cli) = 0;
		virtual double          __fastcall PM_GetTDMes(int n) = 0;
		virtual void            __fastcall PM_SetTDMes(int n, double tdesl) = 0;
		virtual double          __fastcall PM_GetDuraTotalMes(int n) = 0;
		virtual void            __fastcall PM_SetDuraTotalMes(int n, double duraTotMes) = 0;
		virtual double          __fastcall PM_GetX(void) = 0; //hk201702 coordenadas da chave ou da carga
		virtual void            __fastcall PM_SetX(double x) = 0;
		virtual double          __fastcall PM_GetY(void) = 0;
		virtual void            __fastcall PM_SetY(double y) = 0;
		virtual TList*          __fastcall PM_GetLisParamCarga(void) = 0; //hk201703: adaptacoes planTurmas
		virtual TList*          __fastcall PM_GetLisParamBloco(void) = 0; //hk201703: adaptacoes planTurmas
		virtual double          __fastcall PM_GetTat(void) = 0;           //hk201703: Correção do tempo de atendimento
		virtual void            __fastcall PM_SetTat(double tat) = 0; //hk201703: Correção do tempo de atendimento
		virtual double          __fastcall PM_GetPtpre(void) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		virtual void            __fastcall PM_SetPtpre(double ptpre) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		virtual double          __fastcall PM_GetPtdlo(void) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		virtual void            __fastcall PM_SetPtdlo(double ptdlo) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		virtual double          __fastcall PM_GetPtexe(void) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
		virtual void            __fastcall PM_SetPtexe(double ptexe) = 0;//hk201505: acréscimo de percentuais do tat (prep, desl+loc, exe)
      //hk201706: multiplas bases módulo de planejamento de turmas
      //: estatísticas por base de atendimento de tma das ocorrências e tma por simulação
		virtual double          __fastcall PM_GetTMABase(void) = 0;
		virtual void            __fastcall PM_SetTMABase(double tmaBase) = 0;
		virtual double          __fastcall PM_GetTMAReal(void)  = 0;
		virtual void            __fastcall PM_SetTMAReal(double tmaReal) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTBarraOc
//---------------------------------------------------------------------------
VTBarraOc* __fastcall NewObjBarraOc(void);
//---------------------------------------------------------------------------
#endif
