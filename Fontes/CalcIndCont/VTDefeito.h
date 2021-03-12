//---------------------------------------------------------------------------
#ifndef VTDefeitoH
#define VTDefeitoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTBloco;
class VTChave;
class VTEqpto;
class VTEstratSoc;
class VTRede;
class strIF;
//---------------------------------------------------------------------------
struct strManobra
					 {
					 double tempo;
					 int    nBlocosManobra;
					 double ndef;
					 double clih, cli; //impacto da manobra
					 VTChave *chaveAbre, *chaveFecha;
					 };
//---------------------------------------------------------------------------
class VTDefeito: public TObject
	{
	public:  //property
		__property VTEqpto*      Eqpto               = {read=PM_GetEqpto, write=PM_SetEqpto};
		__property VTRede*       Rede                = {read=PM_GetRede, write=PM_SetRede};
		__property VTBloco*      Bloco               = {read=PM_GetBloco, write=PM_SetBloco};
		__property VTChave*      ChvProt             = {read=PM_GetChaveProt, write=PM_SetChaveProt};
		__property VTChave*      ChvReliga           = {read=PM_GetChaveReliga, write=PM_SetChaveReliga};
		__property VTChave*      ChvIsolaMont        = {read=PM_GetChaveIsolaMont, write=PM_SetChaveIsolaMont};
		__property VTChave*      ChvIsolaAutoMont    = {read=PM_GetChaveIsolaAutoMont, write=PM_SetChaveIsolaAutoMont};
		__property TList*        LisProt             = {read=PM_GetLisProt};
		__property TList*        LisChvMon           = {read=PM_GetLisChvMon};
		__property TList*        LisChvJus           = {read=PM_GetLisChvJus};
		__property TList*        LisBlocosMan        = {read=PM_GetLisBlocosMan};
		__property TList*        LisChavesBlocosMan  = {read=PM_GetLisChavesBlocosMan};
		__property TList*        LisBlocoManVis      = {read=PM_GetLisBlocosManVis};
		__property TList*        LisBlocosVis        = {read=PM_GetLisBlocosVis};
		__property TList*        LisChvAlivioVis     = {read=PM_GetLisChvAlivioVis};
		__property TList*        LisChvBlocoVis      = {read=PM_GetLisChvBlocoVis};
		__property TList*        LisChvVisDef        = {read=PM_GetLisChvVisDef};
		__property TList*        LisVisRedeDef       = {read=PM_GetLisVisRedeDef};
		__property TList*        LisBlocoRadJusDef   = {read=PM_GetLisBlocoRadJusDef};
		__property TList*        LisBlocosJusProt    = {read=PM_GetLisBlocosJusProt};
		__property TList*        LisBlocosJusDef     = {read=PM_GetLisBlocosJusDef};
		__property TList*        LisBlocosProtDef    = {read=PM_GetLisBlocosProtDef};
		__property TList*        LisBlocosJusAuto    = {read=PM_GetLisBlocosJusAuto};
		__property TList*        LisBlocosProtAuto   = {read=PM_GetLisBlocosProtAuto};
		__property TList*        LisBlocosAutoDef    = {read=PM_GetLisBlocosAutoDef};
		__property TList*        LisBlocosAgrega     = {read=PM_GetLisBlocosAgrega};
		__property TList*        LisLigaVis          = {read=PM_GetLisLigaVis};
		__property TList*        LisLigaVisDef       = {read=PM_GetLisLigaVisDef};
		__property TDateTime     Hsist               = {read=PM_GetHSist, write=PM_SetHSist};
		__property TDateTime     HpreDef             = {read=PM_GetHpreDef, write=PM_SetHpreDef};
		__property TDateTime     HfimDef             = {read=PM_GetHfimDef, write=PM_SetHfimDef};
		__property double        Durarep             = {read=PM_GetDuraRep, write=PM_SetDuraRep};
		__property double        DuraPreFalta        = {read=PM_GetDuraPreFalta, write=PM_SetDuraPreFalta};
		__property int           IPat                = {read=PM_GetIPat, write=PM_SetIPat};
		__property double        FC                  = {read=PM_GetFC, write=PM_SetFC};
		__property int           IniPat              = {read=PM_GetIniPat, write=PM_SetIniPat};
		__property int           FimPat              = {read=PM_GetFimPat, write=PM_SetFimPat};
		__property VTEstratSoc*  EstratSoc[int np]   = {read=PM_GetEstratSoc};
		__property double        NFalhas             = {read=PM_GetNFalhas, write=PM_SetNFalhas};
		__property double        NPisca              = {read=PM_GetNPisca, write=PM_SetNPisca};
		__property double        TReparo             = {read=PM_GetTReparo, write=PM_SetTReparo};
		__property double        TReparoTotal        = {read=PM_GetTReparoTotal, write=PM_SetTReparoTotal};
		__property double        TaxaFalha           = {read=PM_GetTaxaFalha, write=PM_SetTaxaFalha};
		__property double        ComprimentoBloco    = {read=PM_GetComprimentoBloco, write=PM_SetComprimentoBloco};
		__property double        DCons               = {read=PM_GetDCons, write=PM_SetDCons};
		__property double        TCons               = {read=PM_GetTCons, write=PM_SetTCons};
		__property double        NmanAuto            = {read=PM_GetNmanAuto, write=PM_SetNmanAuto};//hkedp int->double
		__property double        NmanMan             = {read=PM_GetNmanMan, write=PM_SetNmanMan};  //hkedp int->double
		__property bool          FlagCortaVis        = {read=PM_GetFlagCortaVis, write=PM_SetFlagCortaVis};
		__property double        FatTMA              = {read=PM_GetFatTMA, write=PM_SetFatTMA};  //hk201703: adaptações para planTurmas
     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
		__property bool          FlagTPre           = {read=PM_GetFlagTPre};
		__property double        Ptpre              = {read=PM_GetPtpre, write=PM_SetPtpre};
		__property double        Ptdlo              = {read=PM_GetPtdlo, write=PM_SetPtdlo};
		__property double        Ptexe              = {read=PM_GetPtexe, write=PM_SetPtexe};
     //hk201710IF: Informações de identificadores de falta, IF
		__property strIF*        InfIF              = {read=PM_GetInfIF, write=PM_SetInfIF};
		__property double        FatIFDlo           = {read=PM_GetFatIFDlo, write=PM_SetFatIFDlo};
		__property double        FatIFPre           = {read=PM_GetFatIFPre, write=PM_SetFatIFPre};
		//__property TList*        LisJusVisSoc        = {read=PM_GetLisJusVisSoc};
		//__property TList*        LisBlocosSoc[int np]  = {read=PM_GetLisBlocosSoc};
		//__property TList*        LisVisRedeSoc[int np] = {read=PM_GetLisVisRedeSoc};
		//__property double        CargaTransf[int np]   = {read=PM_GetCargaTransf, write=PM_SetCargaTransf};
		//__property double        CapacTransf[int np]   = {read=PM_GetCapacTransf, write=PM_SetCapacTransf};

	public:
									__fastcall VTDefeito(void) {};
									__fastcall ~VTDefeito(void) {};
			  virtual	 void __fastcall Inicia(void) = 0;
			  virtual	 void __fastcall ReiniciaTempos(void) = 0;
			  virtual  double __fastcall ContribTransf(void) = 0; //Contribuicao em % (clihSemTransf/ClihComTRansf)

	protected: //métodos acessados via property
	  virtual VTEqpto*     __fastcall PM_GetEqpto(void) = 0;
	  virtual void         __fastcall PM_SetEqpto(VTEqpto *eqpto) = 0;
	  virtual VTRede*      __fastcall PM_GetRede(void) = 0;
	  virtual void         __fastcall PM_SetRede(VTRede *rede) = 0;
	  virtual VTBloco*     __fastcall PM_GetBloco(void) = 0;
	  virtual void         __fastcall PM_SetBloco(VTBloco *bloco) = 0;
	  virtual VTChave*     __fastcall PM_GetChaveProt(void) = 0;
	  virtual void         __fastcall PM_SetChaveProt(VTChave *chvProt) = 0;
	  virtual VTChave*     __fastcall PM_GetChaveReliga(void) = 0;
	  virtual void         __fastcall PM_SetChaveReliga(VTChave *chvReliga) = 0;
	  virtual VTChave*     __fastcall PM_GetChaveIsolaMont(void) = 0;
	  virtual void         __fastcall PM_SetChaveIsolaMont(VTChave *chvIsolaMont) = 0;
	  virtual VTChave*     __fastcall PM_GetChaveIsolaAutoMont(void) = 0;
	  virtual void         __fastcall PM_SetChaveIsolaAutoMont(VTChave *chvIsolaAutoMont) = 0;
	  virtual TList*       __fastcall PM_GetLisProt(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChvMon(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChvJus(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosMan(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChavesBlocosMan(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosManVis(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosVis(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChvAlivioVis(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChvBlocoVis(void) = 0;
	  virtual TList*       __fastcall PM_GetLisChvVisDef(void) = 0;
	  virtual TList*       __fastcall PM_GetLisVisRedeDef(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocoRadJusDef(void)  = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosJusProt(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosJusDef(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosProtDef(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosJusAuto(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosProtAuto(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosAutoDef(void) = 0;
	  virtual TList*       __fastcall PM_GetLisBlocosAgrega(void)= 0;
	  virtual TList*       __fastcall PM_GetLisLigaVis(void)= 0;
	  virtual TList*       __fastcall PM_GetLisLigaVisDef(void)= 0;
	  virtual TDateTime    __fastcall PM_GetHSist(void) = 0;
	  virtual void         __fastcall PM_SetHSist(TDateTime hsist) = 0;
	  virtual TDateTime    __fastcall PM_GetHpreDef(void) = 0;
	  virtual void         __fastcall PM_SetHpreDef(TDateTime hpreDef) = 0;
	  virtual TDateTime    __fastcall PM_GetHfimDef(void) = 0;
	  virtual void         __fastcall PM_SetHfimDef(TDateTime hfimDef) = 0;
	  virtual double       __fastcall PM_GetDuraRep(void) = 0;
	  virtual void         __fastcall PM_SetDuraRep(double durarep) = 0;
	  virtual double       __fastcall PM_GetDuraPreFalta(void) = 0;
	  virtual void         __fastcall PM_SetDuraPreFalta(double duraPreFalta) = 0;
	  virtual int          __fastcall PM_GetIPat(void) = 0;
	  virtual void         __fastcall PM_SetIPat(int iPat) = 0;
	  virtual double       __fastcall PM_GetFC(void) = 0;
	  virtual void         __fastcall PM_SetFC(double fc) = 0;
	  virtual int          __fastcall PM_GetIniPat(void) = 0;
	  virtual void         __fastcall PM_SetIniPat(int iniPat) = 0;
	  virtual int          __fastcall PM_GetFimPat(void) = 0;
	  virtual void         __fastcall PM_SetFimPat(int fimPat) = 0;
	  virtual VTEstratSoc* __fastcall PM_GetEstratSoc(int np) = 0;
	  virtual double       __fastcall PM_GetNFalhas(void) = 0;
	  virtual void         __fastcall PM_SetNFalhas(double nfalhas) = 0;
	  virtual double       __fastcall PM_GetNPisca(void) = 0;
	  virtual void         __fastcall PM_SetNPisca(double npisca) = 0;
	  virtual double       __fastcall PM_GetTReparo(void) = 0;
	  virtual void         __fastcall PM_SetTReparo(double treparo) = 0;
	  virtual double       __fastcall PM_GetTReparoTotal(void) = 0;
	  virtual void         __fastcall PM_SetTReparoTotal(double treparoTotal) = 0;
	  virtual double       __fastcall PM_GetTaxaFalha(void) = 0;
	  virtual void         __fastcall PM_SetTaxaFalha(double taxaFalha) = 0;
	  virtual double       __fastcall PM_GetComprimentoBloco(void) = 0;
	  virtual void         __fastcall PM_SetComprimentoBloco(double comprimentoBloco) = 0;
	  virtual double       __fastcall PM_GetDCons(void) = 0;
	  virtual void         __fastcall PM_SetDCons(double dcons) = 0;
	  virtual double       __fastcall PM_GetTCons(void) = 0;
	  virtual void         __fastcall PM_SetTCons(double tcons) = 0;
	  virtual double       __fastcall PM_GetNmanAuto(void) = 0;   				//hkedp int->double
	  virtual void         __fastcall PM_SetNmanAuto(double nmanAuto) = 0;	//hkedp int->double
	  virtual double       __fastcall PM_GetNmanMan(void) = 0;					//hkedp int->double
	  virtual void         __fastcall PM_SetNmanMan(double nmanMan) = 0;		//hkedp int->double
	  virtual bool         __fastcall PM_GetFlagCortaVis(void) = 0;
	  virtual void         __fastcall PM_SetFlagCortaVis(bool flagCortaVis) = 0;
	  virtual double       __fastcall PM_GetFatTMA(void)= 0;//hk201703: adaptações para planTurmas
	  virtual void         __fastcall PM_SetFatTMA(double fatTMA)= 0;//hk201703: adaptações para planTurmas
     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
	  virtual bool         __fastcall PM_GetFlagTPre(void) = 0;
	  virtual double       __fastcall PM_GetPtpre(void) = 0;
	  virtual void         __fastcall PM_SetPtpre(double ptpre) = 0;
	  virtual double       __fastcall PM_GetPtdlo(void) = 0;
	  virtual void         __fastcall PM_SetPtdlo(double ptdlo) = 0;
	  virtual double       __fastcall PM_GetPtexe(void) = 0;
	  virtual void         __fastcall PM_SetPtexe(double ptexe) = 0;
     //hk201710IF: Informações de identificadores de falta, IF
	  virtual strIF*       __fastcall PM_GetInfIF(void) = 0;
	  virtual void         __fastcall PM_SetInfIF(strIF *infIF) = 0;
	  virtual double       __fastcall PM_GetFatIFDlo(void) = 0;
	  virtual void         __fastcall PM_SetFatIFDlo(double fatIFDlo) = 0;
	  virtual double       __fastcall PM_GetFatIFPre(void) = 0;
	  virtual void         __fastcall PM_SetFatIFPre(double fatIFPre) = 0;
	  //virtual TList*       __fastcall PM_GetLisJusVisSoc(void) = 0;
	  //virtual TList*       __fastcall PM_GetLisBlocosSoc(int np) = 0;
	  //virtual TList*       __fastcall PM_GetLisVisRedeSoc(int np) = 0;
	  //virtual double       __fastcall PM_GetCargaTransf(int np) = 0;
	  //virtual void         __fastcall PM_SetCargaTransf(int np, double cargaTransf) = 0;
	  //virtual double       __fastcall PM_GetCapacTransf(int np) = 0;
	  //virtual void         __fastcall PM_SetCapacTransf(int np, double capacTransf) = 0;

	public:
	  double deltaDEC, deltaFEC, deltaEND;
	  double ganhoTransf; //ganho percentual da transferencia
     double tlocaliza, texecucao, tman_montante, tman_jusante;
	  strManobra ManProtAut, ManAutoDef, ManProtDef;
	  strManobra ManReparo[24];
	  strManobra ManSocRapida[24];
	  strManobra ManSocLenta[24];


	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTDefeito* __fastcall NewObjDefeito(VTBloco *blocoDef, int numPat);

//---------------------------------------------------------------------------
#endif

